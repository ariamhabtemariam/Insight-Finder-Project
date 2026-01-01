#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>

// ------------------------------------------------------------
// Constructor / destructor
// ------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Attribute choices for custom insights + heatmap
    // (labels can be whatever you like, we map them to internal keys)
    QStringList attrs = {
        "Primary OS",
        "Study Time",
        "Favorite Color",
        "Hobby",
        "Region",
        "Language",
        "Engineering Focus",
        "Course Load",
        "Graduation Year"
    };

    ui->comboAttrX->addItems(attrs);
    ui->comboAttrY->addItems(attrs);

    ui->comboHeatX->addItems(attrs);
    ui->comboHeatY->addItems(attrs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ------------------------------------------------------------
// Helper: map combo box label -> InsightGenerator attribute key
// ------------------------------------------------------------

std::string MainWindow::comboToKey(const QString &label)
{
    QString L = label.toLower();

    if (L.contains("os"))          return "os";
    if (L.contains("study"))       return "study";
    if (L.contains("color"))       return "color";
    if (L.contains("hobby"))       return "hobby";
    if (L.contains("region"))      return "region";
    if (L.contains("language"))    return "language";
    if (L.contains("focus"))       return "focus";
    if (L.contains("course"))      return "course";
    if (L.contains("graduation"))  return "graduation";
    if (L.contains("year"))        return "graduation";

    // fallback: just lowercase text, InsightGenerator will normalize
    return L.toStdString();
}

// ------------------------------------------------------------
// Helper: extract attribute values for heatmap (mirrors InsightGenerator)
// ------------------------------------------------------------

std::vector<std::string> MainWindow::extractAttrValues(const Person &person,
                                                       const std::string &attrKey)
{
    // Here we assume attrKey already like: "os", "study", "color", ...
    // (we normalize in comboToKey)
    if (attrKey == "os") {
        PrimaryOS os = person.getPrimaryOS();
        if (os == PrimaryOS::Unknown) return {};
        return { to_string(os) };
    }
    else if (attrKey == "study") {
        StudyTime st = person.getStudyTime();
        if (st == StudyTime::Unknown) return {};
        return { to_string(st) };
    }
    else if (attrKey == "color") {
        const auto &colors = person.getFavoriteColors();
        return std::vector<std::string>(colors.begin(), colors.end());
    }
    else if (attrKey == "hobby") {
        const auto &hobbies = person.getHobbies();
        return std::vector<std::string>(hobbies.begin(), hobbies.end());
    }
    else if (attrKey == "region") {
        Region r = person.getRegion();
        if (r == Region::Unknown) return {};
        return { to_string(r) };
    }
    else if (attrKey == "language") {
        const auto &langs = person.getLanguages();
        return std::vector<std::string>(langs.begin(), langs.end());
    }
    else if (attrKey == "focus") {
        EngineeringFocus f = person.getEngineeringFocus();
        if (f == EngineeringFocus::Unknown) return {};
        return { to_string(f) };
    }
    else if (attrKey == "course") {
        int load = person.getCourseLoad();
        if (load <= 0) return {};
        return { std::to_string(load) };
    }
    else if (attrKey == "graduation") {
        int year = person.getGraduationYear();
        if (year <= 0) return {};
        return { std::to_string(year) };
    }

    return {};
}

// ------------------------------------------------------------
// CSV loading
// ------------------------------------------------------------

void MainWindow::on_btnBrowseCsv_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Choose CSV file",
        "",
        "CSV Files (*.csv)"
        );

    if (!fileName.isEmpty())
        ui->lineCsvPath->setText(fileName);
}

void MainWindow::on_btnLoadCsv_clicked()
{
    QString path = ui->lineCsvPath->text();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a CSV file first.");
        return;
    }

    try {
        PersonCsvReader reader(path.toStdString());
        m_persons = reader.read();
        refreshPeopleTable();
        QMessageBox::information(this, "Loaded", "CSV loaded successfully.");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "CSV Error", e.what());
    }
}

// ------------------------------------------------------------
// JSON loading (via PersonJsonReader + curl)
// ------------------------------------------------------------

void MainWindow::on_btnFetchJson_clicked()
{
    QString url = ui->lineJsonUrl->text();
    if (url.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter a JSON URL first.");
        return;
    }

    try {
        PersonJsonReader reader(url.toStdString());
        m_persons = reader.read();
        refreshPeopleTable();
        QMessageBox::information(this, "Loaded", "JSON loaded successfully.");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "JSON Error", e.what());
    }
}

// ------------------------------------------------------------
// People table
// ------------------------------------------------------------

void MainWindow::refreshPeopleTable()
{
    ui->tablePeople->clear();
    ui->tablePeople->setRowCount(static_cast<int>(m_persons.size()));
    ui->tablePeople->setColumnCount(7);

    ui->tablePeople->setHorizontalHeaderLabels({
        "ID","Year","Region","OS","Study Time","Focus","Courses"
    });

    int r = 0;
    for (const auto &p : m_persons) {
        ui->tablePeople->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(p.getId())));
        ui->tablePeople->setItem(r, 1, new QTableWidgetItem(QString::number(p.getGraduationYear())));
        ui->tablePeople->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(to_string(p.getRegion()))));
        ui->tablePeople->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(to_string(p.getPrimaryOS()))));
        ui->tablePeople->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(to_string(p.getStudyTime()))));
        ui->tablePeople->setItem(r, 5, new QTableWidgetItem(QString::fromStdString(to_string(p.getEngineeringFocus()))));
        ui->tablePeople->setItem(r, 6, new QTableWidgetItem(QString::number(p.getCourseLoad())));
        ++r;
    }

    ui->tablePeople->resizeColumnsToContents();
}

// ------------------------------------------------------------
// Default insights (built-in pairs)
// ------------------------------------------------------------

void MainWindow::on_btnGenerateDefault_clicked()
{
    if (m_persons.empty()) {
        QMessageBox::warning(this, "No Data", "Load data first.");
        return;
    }

    auto results = m_generator.generate(m_persons, m_blockedKeys);
    m_currentInsights = results;

    refreshInsightsTable();
    rebuildHeatmap();
}

// ------------------------------------------------------------
// Insights table
// ------------------------------------------------------------

void MainWindow::refreshInsightsTable()
{
    ui->tableInsights->clear();
    ui->tableInsights->setRowCount(static_cast<int>(m_currentInsights.size()));
    ui->tableInsights->setColumnCount(5);

    ui->tableInsights->setHorizontalHeaderLabels(
        {"Key","Description","Score","Support","Population"}
        );

    int r = 0;
    for (const auto &ins : m_currentInsights) {
        ui->tableInsights->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(ins.key)));
        ui->tableInsights->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(ins.description)));
        ui->tableInsights->setItem(r, 2, new QTableWidgetItem(QString::number(ins.score)));
        ui->tableInsights->setItem(r, 3, new QTableWidgetItem(QString::number(static_cast<int>(ins.support))));
        ui->tableInsights->setItem(r, 4, new QTableWidgetItem(QString::number(static_cast<int>(ins.population))));
        ++r;
    }

    ui->tableInsights->resizeColumnsToContents();
}

// ------------------------------------------------------------
// Block / unblock
// ------------------------------------------------------------

void MainWindow::on_btnBlockSelected_clicked()
{
    auto items = ui->tableInsights->selectedItems();
    if (items.isEmpty()) return;

    int row = items.first()->row();
    if (row < 0 || row >= static_cast<int>(m_currentInsights.size())) return;

    m_blockedKeys.insert(m_currentInsights[row].key);
    refreshBlockedList();

    // Re-run current mode: for simplicity, just re-run default generator
    auto newResults = m_generator.generate(m_persons, m_blockedKeys);
    m_currentInsights = newResults;
    refreshInsightsTable();
}

void MainWindow::on_btnUnblock_clicked()
{
    auto items = ui->listBlocked->selectedItems();
    if (items.isEmpty()) return;

    for (auto *i : items) {
        m_blockedKeys.erase(i->text().toStdString());
    }

    refreshBlockedList();

    // Re-run default generator
    auto newResults = m_generator.generate(m_persons, m_blockedKeys);
    m_currentInsights = newResults;
    refreshInsightsTable();
}

void MainWindow::refreshBlockedList()
{
    ui->listBlocked->clear();
    for (const auto &k : m_blockedKeys) {
        ui->listBlocked->addItem(QString::fromStdString(k));
    }
}

// ------------------------------------------------------------
// Save blocked keys
// ------------------------------------------------------------

void MainWindow::on_btnSaveBlocked_clicked()
{
    QString file = QFileDialog::getSaveFileName(
        this, "Save Blocked Keys", "", "Text Files (*.txt)");

    if (file.isEmpty()) return;

    std::ofstream out(file.toStdString());
    for (const auto &k : m_blockedKeys) {
        out << k << "\n";
    }

    QMessageBox::information(this, "Saved", "Blocked keys saved.");
}

// ------------------------------------------------------------
// Export useful insights
// ------------------------------------------------------------

void MainWindow::on_btnExportUseful_clicked()
{
    QString file = QFileDialog::getSaveFileName(
        this, "Save Useful Insights", "", "CSV Files (*.csv)");

    if (file.isEmpty()) return;

    InsightStore s;
    s.saveUseful(m_currentInsights, file.toStdString());

    QMessageBox::information(this, "Saved", "Useful insights exported.");
}

// ------------------------------------------------------------
// Custom insights (real generic X↔Y pair)
// ------------------------------------------------------------

void MainWindow::on_btnGenerateCustom_clicked()
{
    if (m_persons.empty()) {
        QMessageBox::warning(this, "No Data", "Load data first.");
        return;
    }

    QString labelX = ui->comboAttrX->currentText();
    QString labelY = ui->comboAttrY->currentText();

    if (labelX.isEmpty() || labelY.isEmpty()) {
        QMessageBox::warning(this, "Error", "Choose two attributes.");
        return;
    }

    std::string keyX = comboToKey(labelX);
    std::string keyY = comboToKey(labelY);

    if (keyX == keyY) {
        QMessageBox::warning(this, "Invalid Pair",
                             "Please pick two different attributes for custom insights.");
        return;
    }

    auto results = m_generator.generateGeneric(m_persons, m_blockedKeys, keyX, keyY);
    m_currentInsights = results;

    refreshInsightsTable();
    rebuildHeatmap(); // optional: reflect new pair in heatmap too
}

// ------------------------------------------------------------
// Heatmap
// ------------------------------------------------------------

void MainWindow::on_btnUpdateHeatmap_clicked()
{
    rebuildHeatmap();
}

void MainWindow::rebuildHeatmap()
{
    ui->tableHeatmap->clear();

    if (m_persons.empty()) {
        ui->tableHeatmap->setRowCount(0);
        ui->tableHeatmap->setColumnCount(0);
        return;
    }

    // Fixed attribute list (same as CLI discover-all)
    std::vector<std::string> attributes = {
        "os", "study", "color", "hobby",
        "region", "language", "focus", "course", "graduation"
    };

    const int n = static_cast<int>(attributes.size());
    ui->tableHeatmap->setRowCount(n);
    ui->tableHeatmap->setColumnCount(n);

    // Set headers to attribute names
    QStringList headers;
    for (const auto& attr : attributes) {
        headers << QString::fromStdString(attr);
    }
    ui->tableHeatmap->setHorizontalHeaderLabels(headers);
    ui->tableHeatmap->setVerticalHeaderLabels(headers);

    // Build a map of (attrX, attrY) -> average score
    std::map<std::pair<std::string, std::string>, double> scoreMap;

    // Use same blocklist you use for the insight table
    std::unordered_set<std::string> suppressedKeys = m_blockedKeys;

    for (std::size_t i = 0; i < attributes.size(); ++i) {
        for (std::size_t j = 0; j < attributes.size(); ++j) {
            if (i == j) continue; // diagonal handled later

            auto insights = m_generator.generateGeneric(
                m_persons,
                suppressedKeys,
                attributes[i],
                attributes[j]
                );

            if (insights.empty()) {
                continue;
            }

            int totalScore = 0;
            for (const auto& ins : insights) {
                totalScore += ins.score;
            }

            double avgScore =
                static_cast<double>(totalScore) / static_cast<double>(insights.size());

            scoreMap[{attributes[i], attributes[j]}] = avgScore;
        }
    }

    // Fill table with scores and some basic color coding
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem;

            if (row == col) {
                item->setText("--");
                item->setTextAlignment(Qt::AlignCenter);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableHeatmap->setItem(row, col, item);
                continue;
            }

            auto key = std::make_pair(
                attributes[row],
                attributes[col]
                );

            auto it = scoreMap.find(key);
            if (it == scoreMap.end()) {
                // no data for this pair
                item->setText("");
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableHeatmap->setItem(row, col, item);
                continue;
            }

            int scoreInt = static_cast<int>(std::round(it->second));
            item->setText(QString::number(scoreInt));
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            // Weak / moderate / strong coloring
            QColor bg;
            if (scoreInt > 65) {
                bg = QColor(255, 120, 120);   // strong
            } else if (scoreInt >= 50) {
                bg = QColor(255, 190, 120);   // moderate
            } else {
                bg = QColor(255, 255, 160);   // weak
            }
            item->setBackground(bg);

            ui->tableHeatmap->setItem(row, col, item);
        }
    }

    ui->tableHeatmap->resizeColumnsToContents();
    ui->tableHeatmap->resizeRowsToContents();
}

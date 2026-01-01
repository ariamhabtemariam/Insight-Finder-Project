#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <unordered_set>
#include <string>

#include "Person.h"
#include "PersonCsvReader.h"
#include "PersonJsonReader.h"
#include "PersonRepository.h"
#include "PersonEnums.h"
#include "Insight.h"
#include "InsightGenerator.h"
#include "InsightStore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Data
    std::vector<Person> m_persons;
    std::vector<Insight> m_currentInsights;
    std::unordered_set<std::string> m_blockedKeys;

    InsightGenerator m_generator;

    // Helpers
    void refreshPeopleTable();
    void refreshInsightsTable();
    void refreshBlockedList();
    void rebuildHeatmap();

    std::string comboToKey(const QString &label);
    std::vector<std::string> extractAttrValues(const Person &p, const std::string &attrKey);

private slots:
    // Data loading
    void on_btnBrowseCsv_clicked();
    void on_btnLoadCsv_clicked();
    void on_btnFetchJson_clicked();

    // Insight generation
    void on_btnGenerateDefault_clicked();
    void on_btnGenerateCustom_clicked();

    // Block / unblock / save / export
    void on_btnBlockSelected_clicked();
    void on_btnUnblock_clicked();
    void on_btnSaveBlocked_clicked();
    void on_btnExportUseful_clicked();

    // Heatmap
    void on_btnUpdateHeatmap_clicked();
};

#endif // MAINWINDOW_H

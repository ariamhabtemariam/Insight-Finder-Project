#ifndef PERSONREADER_H
#define PERSONREADER_H

#include "Person.h"
#include <vector>

/**
* Implementors provide a way to populate a collection of Person objects from various sources.
*/
class PersonReader {
public:
    /**
     * Virtual destructor.
     */
    virtual ~PersonReader(){};

    /**
     * Execute a specific read implementation and return a collection of Person objects.
     */
    virtual std::vector<Person> read() = 0;
};

#endif // PERSONREADER_H

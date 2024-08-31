// CMSC 341 - Fall 2022 - Project 4
#include "cache.h"
Cache::Cache(int size, hash_fn hash){
    // Hash formula
    m_hash = hash;
    // Size of table
    m_currentCap = size;
    // Amount of entries in table
    m_currentSize = 0;
    // Number of deleted buckets
    m_currNumDeleted = 0;

    // Creates an empty array of Person objects
    m_currentTable = new Person[m_currentCap];
    // Put an empty Person object into every sloFt
    for (int i=0; i < m_currentCap; i++){
        m_currentTable[i] = Person();
    }

    
    // Initializing variables that won't be used until we reach a load factor of 50% in the current table
    // Size of table
    m_oldCap = 0;
    // Amount of entries
    m_oldSize = 0;
    // Amount of deleted entries
    m_oldNumDeleted = 0;
    // Creates an empty array of Person Objects
    m_oldTable = new Person[m_oldCap];
    // Put an empty Person object into every slot
    for(int i = 0; i < m_oldCap; i++){
        m_oldTable[i] = Person();
    }
    
    m_buildingNewTable = false;
    m_totalNumberDataPoints = 0;
    m_oneQuarter = 0;
}

Cache::~Cache(){
    delete [] m_currentTable;
    delete [] m_oldTable;
}

bool Cache::insert(Person person){
    // Can't insert an ID above 9999, or below 1000
    if(person.getID() > 9999 || person.getID() < 1000){
        return false;
    }

    // index = ((Hash(key) % TableSize) + (i x i)) % TableSize

    // If the bucket is full, will increment
    int quadraticProbe = 0;

    // For storing index value of where to insert in the table
    unsigned int indexValue = -1;

    // Use the hash function to get the index where the key-value pair will be stored in the table
    indexValue = ((m_hash(person.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

    //cout << "String: " << person.getKey() << endl;
    //cout << "indexValue: " << indexValue << endl;

    // While the bucket is not empty
    while(m_currentTable[indexValue].getKey() != ""){
        // Add 1 to the quadratic probe
        quadraticProbe += 1;
        // Run the hash again
        indexValue = ((m_hash(person.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
    }

    // Insert the Person Object at the hashed position
    m_currentTable[indexValue] = person;

    // Increment the current size of the table by one
    m_currentSize++;

    // If the load factor is greater than 0.5, we need to rehash a new table
    if(lambda() > 0.5 && m_buildingNewTable == false){
        //cout << "EPIC" << endl;
        //The capacity of the new table would be the smallest prime number greater than 4 times the current number of 
        //data points. The current number of data points is total number of occupied buckets minus total number 
        // of deleted buckets.

        m_buildingNewTable = true;

        // Total number of occupied buckets minus total number of deleted buckets
        m_totalNumberDataPoints = m_currentSize - m_currNumDeleted;

        int fourTimesNumberOfDataPoints = m_totalNumberDataPoints * 4;

        // Stores the new table size
        int newTableSize = 0;

        // If the value is prime
        if(isPrime(fourTimesNumberOfDataPoints) == true){
            newTableSize = fourTimesNumberOfDataPoints;
        // If the value is not prime, find the next possible prime
        }else{
            newTableSize = findNextPrime(fourTimesNumberOfDataPoints);
        }

        if(newTableSize < 101){
            newTableSize = 101;
        }

        if(newTableSize > 99991){
            newTableSize = 99991;
        }

        // Delete the old array, shove everything from current array into "old" array
        delete [] m_oldTable;

        // Size of table
        m_oldCap = m_currentCap;
        // Amount of entries
        m_oldSize = m_currentSize;
        // Amount of deleted entries
        m_oldNumDeleted = m_currNumDeleted;
        // Creates an array of Person Objects
        m_oldTable = new Person[m_oldCap];
        // Put a Person object into every slot
        for(int i = 0; i < m_oldCap; i++){
            m_oldTable[i] = m_currentTable[i];
        }

        // Delete the current array so we can build a new array
        delete [] m_currentTable;

        // Size of table
        m_currentCap = newTableSize;
        //cout << "m_currentCap: " << m_currentCap << endl;
        // Amount of entries
        m_currentSize = 0;
        // Amount of deleted entries
        m_currNumDeleted = 0;
        // Creates an array of empty Person Objects
        m_currentTable = new Person[m_currentCap];
        // Put an empty Person Object into every slot
        for(int i = 0; i < m_currentSize; i++){
            m_currentTable[i] = Person();
        }

        // Storing what 1/4 of the old table size
        int m_oneQuarter = floor(m_oldSize / 4);
        // Every time we move 1 data point over to the new table, will increase this by one
        int currentAmount = 0;
        // For storing the Person Object that will be moved over
        Person thePerson = Person();

        // Loops through old hash table
        for(int i = 0; i < m_oldCap; i++){
            // If we have moved over 25%, return
            if(currentAmount == m_oneQuarter){
                return true;
            }
            // If the slot is not empty and the bucket is not deleted
            if(m_oldTable[i].getKey() != "" && m_oldTable[i].getKey() != "DELETED"){
                // Store the Person to be inserted into new array
                thePerson = m_oldTable[i];
                // If the bucket is full, will increment
                int quadraticProbe = 0;

                // For storing index value of where to insert in the table
                unsigned int indexValue = -1;

                // Use the hash function to get the index where the key-value pair will be stored in the table
                indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

                // While the bucket is not empty
                while(m_currentTable[indexValue].getKey() != ""){
                    // Add 1 to the quadratic probe
                    quadraticProbe += 1;
                    // Run the hash again
                    indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
                }

                // Insert the Person Object at the hashed position
                m_currentTable[indexValue] = thePerson;

                // Increment the current size of the table by one
                m_currentSize++;

                currentAmount++;

                // Set the key to be deleted in the old table so we don't copy it over again
                m_oldTable[i].setKey("DELETED");
                m_oldTable[i].setID(0);

                m_oldNumDeleted++;
            }
        }
        // We exited the "for loop" without hitting the "currentAmount" condition, so that means we got all the lives nodes out

        
        // Set the boolean back to false
        m_buildingNewTable = false;

        m_totalNumberDataPoints = 0;

        // Delete the old array
        //delete [] m_oldTable;

        // Size of table
        m_oldCap = 0;
        // Amount of entries
        m_oldSize = 0;
        // Amount of deleted entries
        m_oldNumDeleted = 0;
        
        return true;
    // We already made a new table, just transferring data over from the old one
    }else if(m_buildingNewTable == true){
        //cout << "SUIIIII" << endl;
        // Total number of occupied buckets minus total number of deleted buckets
        //int numberOfDataPoints = m_oldSize - m_oldNumDeleted;

        //cout << "m_totalNumberDataPoints: " << m_totalNumberDataPoints;
        // Storing what 1/4 of the number of data points would be
        int oneQuarter = floor(m_totalNumberDataPoints / 4);
        // Every time we move 1 data point over to the new table, will increase this by one
        int currentAmount = 0;
        // For storing the Person Object that will be moved over
        Person thePerson = Person();

        // Loops through old hash table
        for(int i = 0; i < m_oldCap; i++){
            //cout << i << endl;
            //cout << "KEY: " << m_oldTable[i].getKey() << endl;
            // If we have moved over 25%, return
            if(currentAmount == oneQuarter){
                //cout << "oh yeahhhh" << endl;
                return true;
            }
            // If the slot is not empty and the bucket is not deleted
            if(m_oldTable[i].getKey() != "" && m_oldTable[i].getKey() != "DELETED"){
                // Store the Person to be inserted into new array
                thePerson = m_oldTable[i];
                // If the bucket is full, will increment
                int quadraticProbe = 0;

                // For storing index value of where to insert in the table
                unsigned int indexValue = -1;

                // Use the hash function to get the index where the key-value pair will be stored in the table
                indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

                // While the bucket is not empty
                while(m_currentTable[indexValue].getKey() != ""){
                    // Add 1 to the quadratic probe
                    quadraticProbe += 1;
                    // Run the hash again
                    indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
                }

                // Insert the Person Object at the hashed position
                m_currentTable[indexValue] = thePerson;

                // Increment the current size of the table by one
                m_currentSize++;
                //cout << "2: m_currentSize: " << m_currentSize << endl;
 
                //cout << "CURRENT AMOUNT: " << currentAmount << endl;
                currentAmount++;

                // Set the key to be deleted in the old table so we don't copy it over again
                m_oldTable[i].setKey("DELETED");
                m_oldTable[i].setID(0);

                m_oldNumDeleted++;
            }
        }
        // Set the boolean back to false
        m_buildingNewTable = false;

        //cout << "oh boy" << endl;
        //cout << m_oldTable[0].getKey();
        //m_totalNumberDataPoints = 0;

        //m_oldTable[0].setKey("");
        //m_oldTable[0].setID(0);
        // Delete the old array
        //delete [] m_oldTable;
        
        // Size of table
        m_oldCap = 0;
        // Amount of entries
        m_oldSize = 0;
        // Amount of deleted entries
        m_oldNumDeleted = 0;

        
        // Went through the entire table and transferred all of the buckets
        return true;
    }
    // The Person object was inserted
    return true;
}

bool Cache::remove(Person person){
    bool foundPerson = false;

    // If the bucket is full, will increment
    int quadraticProbe = 0;

    // If we have probed 20 times, the Person isn't in the hash table
    int quadraticProbeLimit = 20;

    // For storing index value of where to insert in the table
    unsigned int indexValue = -1;

    // Use the hash function to get the index where the key-value pair will be stored in the table
    indexValue = ((m_hash(person.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

    // While the bucket does not have the id we want
    while(m_currentTable[indexValue].getID() != person.getID() && quadraticProbe <= quadraticProbeLimit){
        // Add 1 to the quadratic probe
        quadraticProbe += 1;
        // Run the hash again
        indexValue = ((m_hash(person.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
    }

    // We found the Person we want
    if(quadraticProbe != quadraticProbeLimit + 1){
        m_currentTable[indexValue].setKey("DELETED");
        m_currentTable[indexValue].setID(0);
        m_currNumDeleted++;
        // cout << "SWEET" << endl;
        foundPerson = true;
    }

    // If we have not found the person we want, look through the old table

    // Reset back to 0 for the old table
    quadraticProbe = 0;

    // Making sure an old table exists
    if(m_oldCap != 0){
         // Use the hash function to get the index where the key-value pair will be stored in the table
        indexValue = ((m_hash(person.getKey()) % m_oldCap) + (quadraticProbe * quadraticProbe)) % m_oldCap;

        // While the bucket does not have the id we want
        while(m_oldTable[indexValue].getID() != person.getID() && quadraticProbe <= quadraticProbeLimit){
            // Add 1 to the quadratic probe
            quadraticProbe += 1;
            // Run the hash again
            indexValue = ((m_hash(person.getKey()) % m_oldCap) + (quadraticProbe * quadraticProbe)) % m_oldCap;
        }

        // We found the Person we want
        if(quadraticProbe != quadraticProbeLimit + 1){
            m_currentTable[indexValue].setKey("DELETED");
            m_currentTable[indexValue].setID(0);
            m_oldNumDeleted++;
            foundPerson = true;
        }
    }

    if(foundPerson == false){
        return false;
    }

    /*
    // Loops through current array
    for(int i = 0; i < m_currentCap; i++){
        // If we find the Person we want
        if(m_currentTable[i].getKey() == person.getKey() && m_currentTable[i].getID() == person.getID()){
            // Set the Person to be a deleted object
            m_currentTable[i] = DELETED;
            return true;
        }
    }

    // Loops through old array
    for(int i = 0; i < m_oldCap; i++){
        // If we find the Person we want
        if(m_oldTable[i].getKey() == person.getKey() && m_oldTable[i].getID() == person.getID()){
            // Set the Person to be a deleted object
            m_oldTable[i] = DELETED;
            return true;
        }
    }
    */


   //cout << "Deleted Ratio: " << deletedRatio() << endl;

    // If the more than 80% of buckets are deleted, we need to rehash a new table
    if(float(deletedRatio()) > float(0.8) && m_buildingNewTable == false){
        //cout << "EPIC" << endl;
        //The capacity of the new table would be the smallest prime number greater than 4 times the current number of 
        //data points. The current number of data points is total number of occupied buckets minus total number 
        // of deleted buckets.

        m_buildingNewTable = true;

        // Total number of occupied buckets minus total number of deleted buckets
        m_totalNumberDataPoints = m_currentSize - m_currNumDeleted;

        //cout << "m_totalNumberDataPoints: " << m_totalNumberDataPoints << endl;

        int fourTimesNumberOfDataPoints = m_totalNumberDataPoints * 4;

        // Stores the new table size
        int newTableSize = 0;

        // If the value is prime
        if(isPrime(fourTimesNumberOfDataPoints) == true){
            newTableSize = fourTimesNumberOfDataPoints;
        // If the value is not prime, find the next possible prime
        }else{
            newTableSize = findNextPrime(fourTimesNumberOfDataPoints);
        }

        if(newTableSize < 101){
            newTableSize = 101;
        }

        if(newTableSize > 99991){
            newTableSize = 99991;
        }

        // Delete the old array, shove everything from current array into "old" array
        delete [] m_oldTable;

        // Size of table
        m_oldCap = m_currentCap;
        // Amount of entries
        m_oldSize = m_currentSize;
        // Amount of deleted entries
        m_oldNumDeleted = m_currNumDeleted;
        // Creates an array of Person Objects
        m_oldTable = new Person[m_oldCap];
        // Put a Person object into every slot
        for(int i = 0; i < m_oldCap; i++){
            m_oldTable[i] = m_currentTable[i];
        }

        // Delete the current array so we can build a new array
        delete [] m_currentTable;

        // Size of table
        m_currentCap = newTableSize;
        // Amount of entries
        m_currentSize = 0;
        // Amount of deleted entries
        m_currNumDeleted = 0;
        // Creates an array of empty Person Objects
        m_currentTable = new Person[m_currentCap];
        // Put an empty Person Object into every slot
        for(int i = 0; i < m_currentSize; i++){
            m_currentTable[i] = Person();
        }

        // Storing what 1/4 of the old table size
        int m_oneQuarter = floor(m_oldSize / 4);
        // Every time we move 1 data point over to the new table, will increase this by one
        int currentAmount = 0;
        // For storing the Person Object that will be moved over
        Person thePerson = Person();

        // Loops through old hash table
        for(int i = 0; i < m_oldCap; i++){
            // If we have moved over 25%, return
            if(currentAmount == m_oneQuarter){
                //cout << "WOO HOO" << endl;
                return true;
            }
            // If the slot is not empty and the bucket is not deleted
            if(m_oldTable[i].getKey() != "" && m_oldTable[i].getKey() != "DELETED"){
                // Store the Person to be inserted into new array
                thePerson = m_oldTable[i];
                // If the bucket is full, will increment
                int quadraticProbe = 0;

                // For storing index value of where to insert in the table
                unsigned int indexValue = -1;

                // Use the hash function to get the index where the key-value pair will be stored in the table
                indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

                // While the bucket is not empty
                while(m_currentTable[indexValue].getKey() != ""){
                    // Add 1 to the quadratic probe
                    quadraticProbe += 1;
                    // Run the hash again
                    indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
                }

                // Insert the Person Object at the hashed position
                m_currentTable[indexValue] = thePerson;

                // Increment the current size of the table by one
                m_currentSize++;

                currentAmount++;

                // Set the key to be deleted in the old table so we don't copy it over again
                m_oldTable[i].setKey("DELETED");
                m_oldTable[i].setID(0);
                //cout << "BOO YUH" << endl;
                m_oldNumDeleted++;
            }
        }
        // We exited the "for loop" without hitting the "currentAmount" condition, so that means we got all the lives nodes out

        // Set the boolean back to false
        m_buildingNewTable = false;

        m_totalNumberDataPoints = 0;

        // Delete the old array
        //delete [] m_oldTable;

        // Size of table
        m_oldCap = 0;
        // Amount of entries
        m_oldSize = 0;
        // Amount of deleted entries
        m_oldNumDeleted = 0;

        return true;
    // We already made a new table, just transferring data over from the old one
    }else if(m_buildingNewTable == true){
        //cout << "ONE. m_totalNumberDataPoints: " << m_totalNumberDataPoints << endl;
        //cout << "NONONONO" << endl;
        // Total number of occupied buckets minus total number of deleted buckets
        //int m_totalNumberDataPoints = m_oldSize - m_oldNumDeleted;

        // Storing what 1/4 of the number of data points would be
        int m_oneQuarter = floor(m_oldSize / 4);
        //cout << "m_totalNumberDataPoints: " << m_totalNumberDataPoints << endl;
        //cout << "oneQuarter: " << oneQuarter << endl;
        // Every time we move 1 data point over to the new table, will increase this by one
        int currentAmount = 0;
        // For storing the Person Object that will be moved over
        Person thePerson = Person();

        // Loops through old hash table
        for(int i = 0; i < m_oldCap; i++){
            //cout << "currentAmount: " << currentAmount << endl;
            // If we have moved over 25%, return
            if(currentAmount == m_oneQuarter){
                //cout << "currentAmounttrue: " << currentAmount << endl;
                return true;
            }
            // If the slot is not empty and the bucket is not deleted
            if(m_oldTable[i].getKey() != "" && m_oldTable[i].getKey() != "DELETED"){
                // Store the Person to be inserted into new array
                thePerson = m_oldTable[i];
                // If the bucket is full, will increment
                int quadraticProbe = 0;

                // For storing index value of where to insert in the table
                unsigned int indexValue = -1;

                // Use the hash function to get the index where the key-value pair will be stored in the table
                indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

                // While the bucket is not empty
                while(m_currentTable[indexValue].getKey() != ""){
                    // Add 1 to the quadratic probe
                    quadraticProbe += 1;
                    // Run the hash again
                    indexValue = ((m_hash(thePerson.getKey()) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
                }

                // Insert the Person Object at the hashed position
                m_currentTable[indexValue] = thePerson;

                // Increment the current size of the table by one
                m_currentSize++;

                currentAmount++;

                // Set the key to be deleted in the old table so we don't copy it over again
                m_oldTable[i].setKey("DELETED");
                m_oldTable[i].setID(0);
                m_oldNumDeleted++;
            }
        }
        // Set the boolean back to false
        m_buildingNewTable = false;

        m_totalNumberDataPoints = 0;

        // Delete the old array
        //delete [] m_oldTable;

        // Size of table
        m_oldCap = 0;
        // Amount of entries
        m_oldSize = 0;
        // Amount of deleted entries
        m_oldNumDeleted = 0;

        // Went through the entire table and transferred all of the buckets
        return true;
    }
    return true;
}

Person Cache::getPerson(string key, int id) const{
    // If the bucket is full, will increment
    int quadraticProbe = 0;

    // If we have probed 20 times, the Person isn't in the hash table
    int quadraticProbeLimit = 20;

    // For storing index value of where to insert in the table
    unsigned int indexValue = -1;

    // Use the hash function to get the index where the key-value pair will be stored in the table
    indexValue = ((m_hash(key) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;

    // While the bucket does not have the id we want
    while(m_currentTable[indexValue].getID() != id && quadraticProbe <= quadraticProbeLimit){
        // Add 1 to the quadratic probe
        quadraticProbe += 1;
        // Run the hash again
        indexValue = ((m_hash(key) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
    }


    // We found the Person we want
    if(quadraticProbe != quadraticProbeLimit + 1){
        return m_currentTable[indexValue];
    }

    // If we have not found the person we want, look through the old table

    // Reset back to 0 for the old table
    quadraticProbe = 0;


    // If an old table exists
    if(m_oldCap != 0){
        // Use the hash function to get the index where the key-value pair will be stored in the table
        indexValue = ((m_hash(key) % m_currentCap) + (quadraticProbe * quadraticProbe)) % m_currentCap;
        // While the bucket does not have the id we want
        while(m_oldTable[indexValue].getID() != id && quadraticProbe <= quadraticProbeLimit){
            // Add 1 to the quadratic probe
            quadraticProbe += 1;
            // Run the hash again
            indexValue = ((m_hash(key) % m_oldCap) + (quadraticProbe * quadraticProbe)) % m_oldCap;
        }

        // We found the Person we want
        if(quadraticProbe != quadraticProbeLimit + 1){
            return m_oldTable[indexValue];
        }
    }

     // If we did not find the Person, return an empty Person Object
    //cout << "NOT FOUND" << endl;
    return Person("",0);

    /*
    // Loops through the table
    for(int i = 0; i < m_currentCap; i++){
        // If we found the correct Person
        if(m_currentTable[i].getKey() == key && m_currentTable[i].getID() == id){
            // Return a Person object
            return Person(key, id);
        }
    }
    // Did not find the person, return an empty Person object
    return Person();
    */
}

float Cache::lambda() const {
    float loadFactor = 0;

    // Occupied buckets (available and deleted) divided by total capacity
    loadFactor = float(m_currentSize) / float(m_currentCap);

    return loadFactor;
}

float Cache::deletedRatio() const {
    float deletedRatio = 0;

    // Deleted buckets divided by occupied buckets (available and deleted)

    deletedRatio = float(m_currNumDeleted) / float(m_currentSize);

    return deletedRatio;
}

void Cache::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool Cache::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int Cache::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Person &person ) {
    if (!person.m_key.empty())
        sout << person.m_key << " (ID " << person.m_id << ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Person& lhs, const Person& rhs){
    return ((lhs.m_key == rhs.m_key) && (lhs.m_id == rhs.m_id));
}
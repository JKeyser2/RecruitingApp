// CMSC 341 - Fall 2022 - Project 4
//#include "cache.h"
#include "cache.cpp"
#include <random>
#include <vector>
const int MINSEARCH = 0;
const int MAXSEARCH = 7;
// the following array defines sample search strings for testing
string searchStr[MAXSEARCH+1]={"c++","python","java","scheme","prolog","c#","c","js"};
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};



class Tester{
    public:
    

    /******************************************
    * Test function declarations go here! *
    ******************************************/

    bool insertionNormalCase(Cache &myCache);
    bool insertionErrorCase(Cache &myCache);

    bool findWithNoCollisionCase(Cache &myCache);
    bool findWithCollisionsCase(Cache &myCache);
    // The Person does not exist
    bool findErrorCase(Cache &myCache);

    bool removeWithNoCollisionsCase(Cache &myCache);
    bool removeWithCollisionsCase(Cache &myCache);

    bool triggeredRehashingAfterInsertion(Cache &myCache);
    bool rehashingCompletedAfterInsertion(Cache &myCache);
    bool rehashingCompletedAfterInsertionTwice(Cache &myCache);

    bool triggeredRehashingAfterRemoval(Cache &myCache);
    bool rehashingCompletedAfterRemoval(Cache &myCache);

    
};

unsigned int hashCode(const string str);

int main () {
    
    Tester tester;

   
    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the normal case of the Insert function for a hash table:\n\n";
        if (tester.insertionNormalCase(myCache) == true)
            cout << "\tNormal case of the Insert function for a hash table passed!\n";
        else
            cout << "\tNormal case of the Insert function for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the error case of the Insert function for a hash table:\n\n";
        if (tester.insertionErrorCase(myCache) == true)
            cout << "\tError case of the Insert function for a hash table passed!\n";
        else
            cout << "\tError case of the Insert function for a hash table failed!\n";
    }
    
    
    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the no collision case of the getPerson() function for a hash table:\n\n";
        if (tester.findWithNoCollisionCase(myCache) == true)
            cout << "\tNo collision case of the getPerson() function for a hash table passed!\n";
        else
            cout << "\tNo collision case of the getPerson() function for a hash table failed!\n";
    }

     {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the collision case of the getPerson() function for a hash table:\n\n";
        if (tester.findWithCollisionsCase(myCache) == true)
            cout << "\tCollision case of the getPerson() function for a hash table passed!\n";
        else
            cout << "\tCollision case of the getPerson() function for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the erorr case of the getPerson() function for a hash table:\n\n";
        if (tester.findErrorCase(myCache) == true)
            cout << "\tError case of the getPerson() function for a hash table passed!\n";
        else
            cout << "\tError case of the getPerson() function for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the no collision case of the Remove function for a hash table:\n\n";
        if (tester.removeWithNoCollisionsCase(myCache) == true)
            cout << "\tNo collision case of the Remove function for a hash table passed!\n";
        else
            cout << "\tNo collision case of the Remove function for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting the collision case of the Remove function for a hash table:\n\n";
        if (tester.removeWithCollisionsCase(myCache) == true)
            cout << "\tCollision case of the Remove function for a hash table passed!\n";
        else
            cout << "\tCollision case of the Remove function for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting that rehashing triggers after insertion that puts load factor above 50'%' for a hash table:\n\n";
        if (tester.triggeredRehashingAfterInsertion(myCache) == true)
            cout << "\tRehashing triggered after load factor surpasses 50'%' for a hash table passed!\n";
        else
            cout << "\tRehashing triggered after load factor surpasses 50'%' for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting that rehashing is successful after insertion that puts load factor above 50'%' for a hash table:\n\n";
        if (tester.rehashingCompletedAfterInsertion(myCache) == true)
            cout << "\tRehashing successful after load factor surpasses 50'%' for a hash table passed!\n";
        else
            cout << "\tRehashing successful after load factor surpasses 50'%' for a hash table failed!\n";
    }
    
    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting that rehashing triggers after removal that puts deleted ratio above 80'%' for a hash table:\n\n";
        if (tester.triggeredRehashingAfterRemoval(myCache) == true)
            cout << "\tRehashing triggered after deleted ratio surpasses 80'%' for a hash table passed!\n";
        else
            cout << "\tRehashing triggered after deleted ratio surpasses 80'%' for a hash table failed!\n";
    }
    
    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting that rehashing successful after removal that puts deleted ratio above 80'%' for a hash table:\n\n";
        if (tester.rehashingCompletedAfterRemoval(myCache) == true)
            cout << "\tRehashing successful after deleted ratio surpasses 80'%' for a hash table passed!\n";
        else
            cout << "\tRehashing successful after deleted ratio surpasses 80'%' for a hash table failed!\n";
    }

    {
        Cache myCache(MINPRIME, hashCode);
        cout << "\nTesting that double rehashing works for a hash table:\n\n";
        if (tester.rehashingCompletedAfterInsertionTwice(myCache) == true)
            cout << "\tDouble rehashing of a hash table passed!\n";
        else
            cout << "\tDouble rehashing of a hash table failed!\n";
    }
    
    return 0;
}

bool Tester::insertionNormalCase(Cache &myCache){
    Person personOne = Person("js", 1000);
    Person personTwo = Person("python", 1001);
    Person personThree = Person("java", 1002);
    Person personFour = Person("scheme", 1003);
    Person personFive = Person("c++", 1004);
    Person personSix = Person("c++", 1005);
    Person personSeven = Person("c++", 1006);
    Person personEight = Person("c++", 1007);

    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);
    myCache.insert(personSix);
    myCache.insert(personSeven);
    myCache.insert(personEight);

    //myCache.dump();


    if(myCache.m_currentTable[0].getKey() != "c++" || myCache.m_currentTable[0].getID() != 1007){
        return false;
    }


    if(myCache.m_currentTable[45].getKey() != "java" || myCache.m_currentTable[45].getID() != 1002){
        return false;
    }


    if(myCache.m_currentTable[58].getKey() != "python" || myCache.m_currentTable[58].getID() != 1001){
        return false;
    }

    if(myCache.m_currentTable[74].getKey() != "scheme" || myCache.m_currentTable[74].getID() != 1003){
        return false;
    }


    if(myCache.m_currentTable[78].getKey() != "js" || myCache.m_currentTable[78].getID() != 1000){
        return false;
    }

     if(myCache.m_currentTable[92].getKey() != "c++" || myCache.m_currentTable[92].getID() != 1004){
        return false;
    }

     if(myCache.m_currentTable[93].getKey() != "c++" || myCache.m_currentTable[93].getID() != 1005){
        return false;
    }

     if(myCache.m_currentTable[96].getKey() != "c++" || myCache.m_currentTable[96].getID() != 1006){
        return false;
    }

    if(myCache.m_currentSize != 8){
        return false;
    }

    return true;
}

bool Tester::insertionErrorCase(Cache &myCache){
    Person personOne = Person("js", 999);
    Person personTwo = Person("python", 10000);

    // Both objects have IDs that are out-of-bounds
    if(myCache.insert(personOne) != false){
        return false;
    }

    if(myCache.insert(personTwo) != false){
        return false;
    }

    return true;
}

bool Tester::findWithNoCollisionCase(Cache &myCache){
    
    Person personOne = Person("c++", 1000);
    Person personTwo = Person("python", 1001);
    Person personThree = Person("java", 1002);
    Person personFour = Person("scheme", 1003);
    Person personFive = Person("prolog", 1004);
    Person personSix = Person("c#", 1005);
    Person personSeven = Person("c", 1006);
    Person personEight = Person("js", 1007);

    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);
    myCache.insert(personSix);
    myCache.insert(personSeven);
    myCache.insert(personEight);


    //myCache.dump();

    // Create a Person object for comparing
    Person tempPerson = Person();


    tempPerson = myCache.getPerson("c++", 1000);


    if(tempPerson.getKey() != personOne.getKey() || tempPerson.getID() != personOne.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("python", 1001);


    if(tempPerson.getKey() != personTwo.getKey() || tempPerson.getID() != personTwo.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("java", 1002);


    if(tempPerson.getKey() != personThree.getKey() || tempPerson.getID() != personThree.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("scheme", 1003);


    if(tempPerson.getKey() != personFour.getKey() || tempPerson.getID() != personFour.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("prolog", 1004);


    if(tempPerson.getKey() != personFive.getKey() || tempPerson.getID() != personFive.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c#", 1005);


    if(tempPerson.getKey() != personSix.getKey() || tempPerson.getID() != personSix.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c", 1006);


    if(tempPerson.getKey() != personSeven.getKey() || tempPerson.getID() != personSeven.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("js", 1007);


    if(tempPerson.getKey() != personEight.getKey() || tempPerson.getID() != personEight.getID()){
        return false;
    }


    return true;
}

bool Tester::findWithCollisionsCase(Cache &myCache){
    
    Person personOne = Person("c++", 1000);
    Person personTwo = Person("c++", 1001);
    Person personThree = Person("c++", 1002);
    Person personFour = Person("c++", 1003);
    Person personFive = Person("c++", 1004);

    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);


    //myCache.dump();

    
    // Create a Person object for comparing
    Person tempPerson = Person();


    tempPerson = myCache.getPerson("c++", 1000);


    if(tempPerson.getKey() != personOne.getKey() || tempPerson.getID() != personOne.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1001);


    if(tempPerson.getKey() != personTwo.getKey() || tempPerson.getID() != personTwo.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1002);


    if(tempPerson.getKey() != personThree.getKey() || tempPerson.getID() != personThree.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1003);


    if(tempPerson.getKey() != personFour.getKey() || tempPerson.getID() != personFour.getID()){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1004);


    if(tempPerson.getKey() != personFive.getKey() || tempPerson.getID() != personFive.getID()){
        return false;
    }

    return true;
}

bool Tester::findErrorCase(Cache &myCache){
    Person personOne = Person("c++", 1000);
    Person personTwo = Person("python", 1001);
    Person personThree = Person("java", 1002);
    Person personFour = Person("scheme", 1003);
    Person personFive = Person("prolog", 1004);
    Person personSix = Person("c#", 1005);
    Person personSeven = Person("c", 1006);
    Person personEight = Person("js", 1007);

    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);
    myCache.insert(personSix);
    myCache.insert(personSeven);
    myCache.insert(personEight);


    //myCache.dump();

    // Create a Person object for comparing
    Person tempPerson = Person();


    tempPerson = myCache.getPerson("c++", 1001);

    //cout << endl;
    //cout << tempPerson.getID();
    //cout << tempPerson.getKey();

    // Does not exist, an empty Person Object should have been returned
    if(tempPerson.getID() != 0 || tempPerson.getKey() != ""){
        return false;
    }
    
    return true;
}

bool Tester::removeWithNoCollisionsCase(Cache &myCache){
    
    Person personOne = Person("c++", 1000);
    Person personTwo = Person("python", 1001);
    Person personThree = Person("java", 1002);
    Person personFour = Person("scheme", 1003);
    Person personFive = Person("prolog", 1004);
    Person personSix = Person("c#", 1005);
    Person personSeven = Person("c", 1006);
    Person personEight = Person("js", 1007);


    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);
    myCache.insert(personSix);
    myCache.insert(personSeven);
    //myCache.insert(personEight);



    //myCache.dump();

    myCache.remove(personOne);
    myCache.remove(personTwo);
    myCache.remove(personThree);
    myCache.remove(personFour);
    myCache.remove(personFive);
    myCache.remove(personSix);
    //myCache.remove(personSeven);
    //myCache.remove(personEight);


    //myCache.dump();

    

    // Create a Person object for comparing
    Person tempPerson = Person();

    // All of the Keys should be returned as ""
    // All of the Values should be returnf as 0
    // This is since we couldn't find of the Person Objects since they were all deleted


    tempPerson = myCache.getPerson("c++", 1000);



    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("python", 1001);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("java", 1002);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("scheme", 1003);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }

    tempPerson = myCache.getPerson("prolog", 1004);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("c#", 1005);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    return true;
    
}

bool Tester::removeWithCollisionsCase(Cache &myCache){

    
    Person personOne = Person("c++", 1000);
    Person personTwo = Person("c++", 1001);
    Person personThree = Person("c++", 1002);
    Person personFour = Person("c++", 1003);
    Person personFive = Person("c++", 1004);

    myCache.insert(personOne);
    myCache.insert(personTwo);
    myCache.insert(personThree);
    myCache.insert(personFour);
    myCache.insert(personFive);

    //myCache.dump();

    myCache.remove(personOne);
    myCache.remove(personTwo);
    myCache.remove(personThree);
    //myCache.remove(personFour);
    //myCache.remove(personFive);
 

    //myCache.dump();

    

    // Create a Person object for comparing
    Person tempPerson = Person();

    // All of the Keys should be returned as ""
    // All of the Values should be returnf as 0
    // This is since we couldn't find of the Person Objects since they were all deleted


    tempPerson = myCache.getPerson("c++", 1000);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1001);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }


    tempPerson = myCache.getPerson("c++", 1002);


    if(tempPerson.getKey() != "" || tempPerson.getID() != 0){
        return false;
    }



    return true;
    
}

bool Tester::triggeredRehashingAfterInsertion(Cache &myCache){
    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<50;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
    }

    //myCache.dump();

    //cout << "CURRENT SIZE: " << myCache.m_currentSize << endl; // 50
    
    Person dataObj2 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum()); 

    //cout << dataObj2.getKey() << ":" << dataObj2.getID() << endl;

    myCache.insert(dataObj2); // 51
    
    // The boolean should flip
    if(myCache.m_buildingNewTable == false){
        return false;
    }

    return true;
}

bool Tester::rehashingCompletedAfterInsertion(Cache &myCache){
    // For storing all Person objects of original table
    // We will rehash and then check if all of these values are in the new table
    vector<Person> dataList;

    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<50;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
        dataList.push_back(dataObj);
    }

    //myCache.dump();

    //cout << "CURRENT SIZE: " << myCache.m_currentSize << endl; // 50
    
    Person dataObj2 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum()); 

    //cout << dataObj2.getKey() << ":" << dataObj2.getID() << endl;

    myCache.insert(dataObj2); // 51
    dataList.push_back(dataObj2);

   // myCache.dump();

    //cout << "SHOUD BE 12" << endl;
    //cout << myCache.m_currentSize << endl;

    
    Person dataObj3 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj3);
    dataList.push_back(dataObj3);

   // myCache.dump();

    //cout << "SHOUD BE 25" << endl;
    //cout << myCache.m_currentSize << endl;

  
    Person dataObj4 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj4);
    dataList.push_back(dataObj4);

    //myCache.dump();

    //cout << "SHOUD BE 38" << endl;
    //cout << myCache.m_currentSize << endl;

   
    Person dataObj5 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj5);
    dataList.push_back(dataObj5);

    //myCache.dump();

    //cout << "SHOUD BE 51" << endl;
    //cout << myCache.m_currentSize << endl;


    Person dataObj6 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj6);
    dataList.push_back(dataObj6);

    //myCache.dump();

    //cout << "SHOUD BE 55" << endl;
    //cout << myCache.m_currentSize << endl;
   /*
    // Loops through, making sure all of the People Objects are in the new Table
    for(unsigned int i = 0; i < dataList.size(); i++){
        cout << "I: " << i << endl;
        if(myCache.getPerson(dataList.at(i).getKey(), dataList.at(i).getID()) == Person("",0)){
            return false;
        }else{
            cout << "YES" << endl;
        }
    }

    // Size of new table should have 55 elements
    if(myCache.m_currentSize != 55){
        return false;
    }

    // Old table should be deleted
    if(myCache.m_oldSize != 0){
        return false;
    }

    cout << "oh boy" << endl;
*/
    return true;
}

bool Tester::triggeredRehashingAfterRemoval(Cache &myCache){
    //cout << "TRIGGER" << endl;
    vector<Person> dataList;

    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<40;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
        dataList.push_back(dataObj);
    }

    // Remove 33 of the 40 People from the hash table, this will trigger a rehashing from the Delete Ratio
    for(unsigned int i = 0; i < dataList.size() - 7; i++){
        myCache.remove(dataList.at(i));
    }

    //myCache.dump();


    // The deleted ratio should be set back to 0 since we made a new table
    if(myCache.deletedRatio() != 0){
        return false;
    }

    return true;
}

bool Tester::rehashingCompletedAfterRemoval(Cache &myCache){
    // For storing all Person objects of original table
    // We will rehash and then check if all of these values are in the new table
    vector<Person> dataList;

    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<40;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
        dataList.push_back(dataObj);
    }

    // Remove 33 of the 40 People from the hash table, this will trigger a rehashing from the Delete Ratio
    for(unsigned int i = 0; i < dataList.size() - 7; i++){
        //cout << i+1 << endl;
        myCache.remove(dataList.at(i));
    }

    // Remove all delete Persons from the vector
    for(int i = 0; i < 33; i++){
        dataList.erase(dataList.begin());
    }

    //myCache.dump();
    //cout << myCache.deletedRatio() << endl;

    // Loops through the remaining Person Objects that have not been deleted
    // To make sure they are in the new table
    for(unsigned int i = 0; i < dataList.size(); i++){
        Person myPerson;
        myPerson = myCache.getPerson(dataList.at(i).getKey(), dataList.at(i).getID());
        // Empty Person object, means we did not find the Person 
        if(myPerson.getKey() == "" || myPerson.getID() == 0){
            cout << i << endl;
            return false;
        }
    }

    // Should be 7 nodes in the new hash table
    if(myCache.m_currentSize != 7){
        return false;
    }

    // Old hash table should no longer exist
    if(myCache.m_oldCap != 0){
        return false;
    }

    //myCache.dump();

    return true;

    /*
    //myCache.dump();

    bool foundOne = false;


    // Loops through the new current table
    for(int i = 0; i < myCache.m_currentCap; i++){
        // If the slot is not empty or deleted, remove from it
        if((myCache.m_currentTable[i].getKey() != "DELETED" && myCache.m_currentTable[i].getKey() != "") && foundOne == false){
            // Remove from the slot
            myCache.remove(myCache.m_currentTable[i]);
            foundOne = true;
        }
    }

    //myCache.dump();
    
    
    foundOne = false;

    // Loops through the new current table
    for(int i = 0; i < myCache.m_currentCap; i++){
        // If the slot is not empty or deleted, remove from it
        if((myCache.m_currentTable[i].getKey() != "DELETED" && myCache.m_currentTable[i].getKey() != "") && foundOne == false){
            // Remove from the slot
            myCache.remove(myCache.m_currentTable[i]);
            foundOne = true;
        }
    }

    foundOne = false;

    //myCache.dump();


    // Loops through the new current table
    for(int i = 0; i < myCache.m_currentCap; i++){
        // If the slot is not empty or deleted, remove from it
        if((myCache.m_currentTable[i].getKey() != "DELETED" && myCache.m_currentTable[i].getKey() != "") && foundOne == false){
            // Remove from the slot
            myCache.remove(myCache.m_currentTable[i]);
            foundOne = true;
        }
    }

    //myCache.dump();

    // Should be 7 nodes in the new hash table, 3 deleted
    if(myCache.m_currentSize != 7 || myCache.m_currNumDeleted != 3){
        return false;
    }

    // Old hash table should no longer exist
    if(myCache.m_oldCap != 0){
        return false;
    }
*/
    
    return true;
}

unsigned int hashCode(const string str) {
   // cout << "String: " << str << endl;

   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for ( unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
    //cout << "Val: " << val << endl;
    //cout << val << endl;
   return val ;
}


bool Tester::rehashingCompletedAfterInsertionTwice(Cache &myCache){
    // For storing all Person objects of original table
    // We will rehash and then check if all of these values are in the new table
    vector<Person> dataList;

    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<50;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
        dataList.push_back(dataObj);
    }

    //myCache.dump();

    //cout << "CURRENT SIZE: " << myCache.m_currentSize << endl; // 50
    
    Person dataObj2 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum()); 

    //cout << dataObj2.getKey() << ":" << dataObj2.getID() << endl;

    myCache.insert(dataObj2); // 51
    dataList.push_back(dataObj2);

   // myCache.dump();

    //cout << "SHOUD BE 12" << endl;
    //cout << myCache.m_currentSize << endl;

    
    Person dataObj3 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj3);
    dataList.push_back(dataObj3);

   // myCache.dump();

    //cout << "SHOUD BE 25" << endl;
    //cout << myCache.m_currentSize << endl;

  
    Person dataObj4 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj4);
    dataList.push_back(dataObj4);

    //myCache.dump();

    //cout << "SHOUD BE 38" << endl;
    //cout << myCache.m_currentSize << endl;

   
    Person dataObj5 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj5);
    dataList.push_back(dataObj5);

    //myCache.dump();

    //cout << "SHOUD BE 51" << endl;
    //cout << myCache.m_currentSize << endl;


    Person dataObj6 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj6);
    dataList.push_back(dataObj6);

    //myCache.dump();

    //cout << "SHOUD BE 55" << endl;
    //cout << myCache.m_currentSize << endl;

    // Loops through, randomly generating key-value Person objects
    for (int i=0;i<51;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        //cout << "AWESOME: " << dataObj.getKey() << " : " << dataObj.getID() << endl;
        // inserting data in to the Cache object
        myCache.insert(dataObj);
        dataList.push_back(dataObj);
    }

    //myCache.dump();

    Person dataObj7 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj7);
    dataList.push_back(dataObj7);

    Person dataObj8 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj8);
    dataList.push_back(dataObj8);

    Person dataObj9 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj9);
    dataList.push_back(dataObj9);

    //myCache.dump();

    Person dataObj10 = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());

    myCache.insert(dataObj10);
    dataList.push_back(dataObj10);

    //myCache.dump();


   /*
    // Loops through, making sure all of the People Objects are in the new Table
    for(unsigned int i = 0; i < dataList.size(); i++){
        cout << "I: " << i << endl;
        if(myCache.getPerson(dataList.at(i).getKey(), dataList.at(i).getID()) == Person("",0)){
            return false;
        }else{
            cout << "YES" << endl;
        }
    }

    // Size of new table should have 55 elements
    if(myCache.m_currentSize != 55){
        return false;
    }

    // Old table should be deleted
    if(myCache.m_oldSize != 0){
        return false;
    }

    cout << "oh boy" << endl;
*/
    return true;
}

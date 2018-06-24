#include <iostream>
#include <cstring>
#include <algorithm>
#include <bitset>
#include "record.h"

using namespace std;

bool judge(char * data){
    return strcmp(data, "Laplace") == 0;
}

int main() {

    char id[10] = "00004";
    char name[10] = "Laplace";
    int age = NULL;
    char major[10] = "Math";

    vector<Column> col = {
            make_tuple("id", DB_STRING, 10, PRIMARY_KEY),
            make_tuple("name", DB_STRING, 10, NONE),
            make_tuple("age", DB_INT, 4, NONE),
            make_tuple("major", DB_STRING, 10, NONE)
    };

    auto table = new Table(1, "student", col);

    vector<char *> arr = {
            id, name, nullptr, major
    };

    vector<string> fields = {"id", "name"};

    auto res = table->query(
            fields,
            *new string("name"),
            judge
    );

    BaseData* data = (*res[0].find("id")).second;

    string LaplaceId = (dynamic_cast<StringData*> (data))->getString();

    cout << LaplaceId << endl;

    return 0;

}

/*
    char id[10] = "00001";
    char name[10] = "Jack";
    int age = 18;
    char major[10] = "Comp.Sci";

    char id[10] = "00002";
    char name[10] = "Franklin";
    int age = 20;
    char major[10] = "Comp.Sci";

    char id[10] = "00003";
    char name[10] = "Winston";
    int age = 22;
    char major[10] = "History";

    char id[10] = "00004";
    char name[10] = "Laplace";
    int age = NULL;
    char major[10] = "Math";

    vector<Column> col = {
            make_tuple("id", DB_STRING, 10, PRIMARY_KEY),
            make_tuple("name", DB_STRING, 10, NONE),
            make_tuple("age", DB_INT, 4, NONE),
            make_tuple("major", DB_STRING, 10, NONE)
    };

    auto table = new Table(1, "student", col);

    table->insertRecord(*new vector<char *> {
        id, name, (char*)&age, major
    });

    auto manager = table->getPageManager();
    manager.flush();
*/

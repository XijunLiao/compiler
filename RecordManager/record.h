//
// Created by Aoleo on 2018/6/7.
//

#ifndef RECORDMANAGER_RECORD_H
#define RECORDMANAGER_RECORD_H

#include <iostream>
#include <map>
#include <vector>
#include "page.h"
#include "dynamic_memory.h"
#include "datatype.h"

using namespace std;

enum DataType {DB_INT, DB_FLOAT, DB_STRING, DB_NULL};

enum DataRule {NONE, PRIMARY_KEY, UNIQUE};

#define AttrArray vector<Attribute>
#define Column tuple<const char *, DataType, int, DataRule>

#define EMPTY_FLAG (-1)
/* Get an int value from the memory space pointed to by a char pointer. */
#define  GET_INT(X) (((int *)(X))[0])
#define  GET_FLOAT(X) (((float *)(X))[0])

#define ATTR_TYPE (0)
#define ATTR_SIZE (1)
#define ATTR_RULE (2)
#define ATTR_OFFSET (3)

class Table
{
private:
    int id;
    string name;
    size_t attrSize;
    size_t nullBitMapSize;

    /**
     * 元组的四个参数分别是数据类型，大小， 约束， 偏移（程序会计算）
     * The four parameters of the tuple are data type, size, constraint, offset (they will be calculated by the program).
     */
    map<string, tuple<DataType, int, DataRule, int>> columns;
    /**
     * 因为MAP是无序的，另外用一个数组来保存属性的顺序
     * Cause map is unordered, using another array is to preserve the order of the columns.
     */
    vector<string> columnNames;

    PageManager* manager;    ///< 用来管理页面，进行底层操作.<br>Used to manage pages and perform low-level operations.
    void addColumn(Column &column);    ///< 私有的方法，用于构造表.<br>Private method, using only for constructor.

    vector<char *> queryAllPri(string& attrName);

    /* Bitmap operation methods */
    void setBitMap(char * source, int size, int position);
    vector<int> getNullPos(char *bitMap);
    vector<string> getNullAttr(char *bitMap);

public:

    Table(int id, const char* name, vector<Column> &columns);
    /**
     * 临时的一个方法，用于测试，实际上Manager应该是被隐藏的。<br>
     * A temporary method for testing only, PageManager should be hidden for others.
     * @todo Remove this method in production context.
     * @return
     */
    PageManager getPageManager();

    /**
     * 从所有记录中删除满足条件的记录。<br>
     * Remove some records from all records.
     * @param attrName The attribute name, used as the subject of the predicate .
     * @param check A function used as a predicate to determine whether to delete.
     */
    void deleteRecord(string& attrName, bool (*check) (BaseData* data));

    /**
     * 按属性名查询所有记录。<br>
     * Query all records by attribute name.
     * @param attrName
     * @return
     */
    vector<map<string, BaseData*>> queryAll(vector<string> &attrNames);

    vector<map<string, BaseData*>> query(vector<string> &attrNames, string &toCheck, bool (*check)(char *));

    /**
     * 用于用户指定了插入属性的键值对时的插入。<br>
     * Insert while using attribute names and attribute values.
     * @return 返回0则插入失败，一般是由于违背了primary key或者unique关系。<br>0 --- failure, generally due to a violation of the primary key or unique rule.
     */
    int insertRecord(map<string, char*> record);

    /**
     * 用于用户没有显式指定键值对应关系，即插入一个表中的所有属性。<br>
     * Insert directly, that means the attribute value corresponds to each column.
     * @return 返回0则插入失败，一般是由于违背了primary key或者unique关系。<br>0 --- failure, generally due to a violation of the primary key or unique rule.
     */
    int insertRecord(vector<char*> &record);
    int modifyRecord(string& attrName, char* newData, bool (*check)(char* data));

    vector<char *> queryPri(string &toCheck, bool (*check)(char *));
};

#endif //RECORDMANAGER_RECORD_H

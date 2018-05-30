#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <QDebug>

//==========================================================
// =大体结构：
//     -- 把文档看作是一个Document对象，它包含了一个Row对象链表。
// = Todo:
//     -- 编辑操作的实现
//     -- 光标的实现
//     --
//     -- 输入：从文件读入，从界面输入
//     -- 输出：向文件输出，显示到界面
//==========================================================

//==================行===================
class Row {
public:
    char *row_text;
    Row *Next_Row;
    Row *Prev_Row;

    int max_len;
    int cur_len;

    Row();
    void add_block();
};

//=================文档光标================
class Cursor {
public:
    Row *hang;
    int row = 0;
    int col = 0;
    int cur_height=0;

    int chi_cnt;
};
//=================整个文档================
class Document {
public:
    bool isModified;
    Row *first_row;
    Row *cur_row;
    Cursor cursor;
    QString file_name;//MODIFIED

    Document();
    void edit(const char *s);
    void add_row(Row *r);
    void delete_row(Row *ptr);
    void read_file(char *file);
    bool save_file(char *file);

    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
    void cursor_home();
    void cursor_end();

    void clear_all();//MODIFIED

};
#endif // DOCUMENT_H

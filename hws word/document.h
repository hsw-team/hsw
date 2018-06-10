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
    int max_len;
    int cur_len;
    char *row_text;
    Row *next_row;
    Row *prev_row;

    Row();
    void expand_block();

};

//=================文档光标================
class Cursor {
public:
    Row *row_ptr;
    int col;
    int row_cnt = 0;
    int cur_height = 0;
    int chi_cnt = 0;
    int eng_cnt = 0;

};

//=================文档对象================
class Document {
public:
    Cursor cursor;
    Row *first_row;
    QString file_name;
    bool isModified;

    Document();
    void clear_all();
    void insert_text(const char *s);
    void new_row(Row *r);
    void delete_row(Row *ptr);
    void read_file(char *file);
    bool save_file(char *file);
    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
    void cursor_home();
    void cursor_end();

};

#endif // DOCUMENT_H

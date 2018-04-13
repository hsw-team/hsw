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

    Row *Next_Row;
    Row *Prev_Row;

    Row();
    void add_block();
    void edit(char *s);
};

//=================整个文档================
class Document {
public:
    Row *first_row;
    Row *cur_row;

    Document();
    void add_row(Row *r);
    void input();
    void read_file(char *file);
    void show_doc();

};

//=================文档光标================
class cursor {
public:
    Row *hang;
    int col;

    cursor();
    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
}

#endif // DOCUMENT_H

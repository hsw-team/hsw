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

    Row();
    void add_block();
    void edit(char *s);
};
Row::Row()
{
    qDebug() << " Row 构造函数\n";
    this->row_text = (char*)calloc(1, sizeof(char) * 100);
    this->Next_Row = NULL;
    this->cur_len = 0;
    this->max_len = 100;

}
void Row::add_block()
{
    qDebug() << "调用 add_block()\n";

    this->row_text = (char*)realloc(row_text, this->max_len + 100 * sizeof(char));

    this->max_len += 100;
}
void Row::edit(char *s)
{
    qDebug() << "调用 edit()\n";
    int s_len = strlen(s);
    while (s_len > (this->max_len - this->cur_len + 1)) {
        this->add_block();
        qDebug() << "edit循环\n";
    }
    if (this->cur_len == 0) {
        strcpy(this->row_text, s);
    }
    else {
        strcat(this->row_text, s);
    }
}

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
Document::Document()
{
    qDebug() << " Document 构造函数\n";
    this->first_row = new Row;
    this->cur_row = this->first_row;
}
void Document::add_row(Row *r)
{
    Row *tmp = new Row;
    r->Next_Row = tmp;
}
void Document::show_doc()
{
    qDebug() << "调用 show_doc()\n";
    Row *ptr = this->first_row;
    while (ptr) {
        qDebug() << "show_doc 循环\n";
        qDebug() << ptr->row_text << endl;
        ptr = ptr->Next_Row;
    }
}

//Qt里这两个还不能直接用
/*
void Document::input()
{
    qDebug() << "调用 input()\n";
    char a[1000];              //a的大小?还需要改改
    Row *ptr = this->first_row;
    while (cin >> a) {
        ptr->edit(a);
        this->add_row(ptr);
        ptr = ptr->Next_Row;
    }

}
//读取文件到document对象，文件路径为char *s;
void Document::read_file(char *s)
{
    string text;
    ifstream myfile(s);
    while (getline(myfile,text)) {
        this->cur_row->edit((char*)text.data());
        this->add_row(cur_row);
        cur_row = cur_row->Next_Row;
    }
}*/

#endif // DOCUMENT_H

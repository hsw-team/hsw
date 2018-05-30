#include "document.h"
#include "mainwindow.h"
Row::Row()
{
    qDebug() << " Row 构造函数\n";
    this->row_text = (char*)calloc(1, sizeof(char) * 100);
    this->Next_Row = NULL;
    this->Prev_Row = NULL;
    this->cur_len = 0;
    this->max_len = 100;

}
void Row::add_block()
{
    qDebug() << "调用 add_block()\n";

    this->row_text = (char*)realloc(row_text, this->max_len + 100 * sizeof(char));

    this->max_len += 100;
}

//=========================================================================================

Document::Document()
{
    qDebug() << " Document 构造函数\n";
    //创建第一行
    this->first_row = new Row;
    this->cursor.hang = this->first_row;
    this->first_row->Prev_Row = NULL;
    //光标初始化
    cursor.hang = first_row;
    cursor.col = 0;
    cursor.row = 0;
    cursor.chi_cnt = 0;
    //文档逻辑
    isModified = false;
    file_name = "";
}
void Document::edit(const char *s)
{
    this->isModified = true;
    qDebug() << "***DOCUMNET*** 调用 edit()\n";
    int s_len = strlen(s);
    while (s_len >= (this->cursor.hang->max_len - this->cursor.hang->cur_len)) {
        this->cursor.hang->add_block();
        qDebug() << "edit循环";
    }
    this->cursor.hang->cur_len += s_len;

    for(int i = cursor.hang->cur_len - 1; i >= cursor.col; i--)
    {
        if(cursor.hang->cur_len + s_len > cursor.hang->max_len)
           cursor.hang->add_block();
        cursor.hang->row_text[i + s_len] = cursor.hang->row_text[i];
    }
    for(int j = 0; j < s_len; j++){
        cursor.hang->row_text[j + cursor.col] = s[j];
    }
    this->cursor.col += s_len;
}

void Document::add_row(Row *ptr)
{
    this->isModified = true;
    Row *tmp = new Row;
    tmp->Prev_Row = ptr;
    tmp->Next_Row = ptr->Next_Row;
    if(ptr->Next_Row)
        ptr->Next_Row->Prev_Row = tmp;
    ptr->Next_Row = tmp;
    ptr = tmp;
}

void Document::delete_row(Row *ptr)
{
    this->isModified = true;
    Row *tmp = new Row;
    tmp = ptr->Prev_Row;
    tmp->Next_Row = ptr->Next_Row;
    if(ptr->Next_Row)
        ptr->Next_Row->Prev_Row = tmp;
    delete ptr;
}

//读取文件到document对象，文件路径为char *s;
void Document::read_file(char *s)
{
    qDebug() << "调用 read_file 函数";
    QFile inputFile(s);
    QString line_text;
    string str;
    if(inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        in.setCodec(QTextCodec::codecForName("gbk"));
        while(in.readLineInto(&line_text)){
            qDebug()<<line_text;
            str = line_text.toStdString();
            this->edit((char*)str.c_str());
            this->add_row(this->cursor.hang);
            this->cursor.cur_height++;
            qDebug() << "height row"<<this->cursor.cur_height << this->cursor.row;
            this->cursor_down();
        }
        inputFile.close();
    }
    this->isModified = false;
}

bool Document::save_file(char *s)
{
    qDebug() << "调用 read_file 函数";
    QFile outputFile(s);
    QString line_text;
    Row *temp = this->first_row;
    if(outputFile.open(QIODevice::WriteOnly | QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out.setCodec(QTextCodec::codecForName("gbk"));
        while(temp)
        {
            line_text = temp->row_text;
            line_text.toStdString();
            out << line_text;
            out << '\n';
            temp = temp->Next_Row;
        }
        outputFile.close();
        this->isModified = false;
        return true;
    }
    else
    {
        return false;
    }
}

//===========================================================================================


void Document::cursor_left()
{
    qDebug() << "@Cursor Left";
    if(cursor.col == 0){
        if(cursor.hang->Prev_Row){
            cursor.hang = cursor.hang->Prev_Row;
            cursor.col = cursor.hang->cur_len;
            cursor.row--;
        }
    }
    else{
        //判断中文字符(中文的ASCII小于0)
        if(cursor.hang->row_text[cursor.col - 1] > 0)
            cursor.col--;
        else{
            cursor.col -= 3;
        }
    }
}
void Document::cursor_right()
{
    qDebug() << "@Cursor Right";
    if(cursor.col >= cursor.hang->cur_len){
        if(cursor.hang->Next_Row){
            cursor.hang = cursor.hang->Next_Row;
            cursor.col = 0;
            cursor.row++;
        }
        else
        {
            cursor.col = cursor.hang->cur_len;
        }
    }
    else{
        //判断中文字符(中文的ASCII小于0)
        if(cursor.hang->row_text[cursor.col] > 0)
            cursor.col++;
        else{
            cursor.col += 3;
        }
    }
}
void Document::cursor_up()//MODIFIED
{
    qDebug() << "@Cursor up";
    int n = cursor.col;
    if(cursor.hang->Prev_Row){
        cursor.hang = cursor.hang->Prev_Row;
        if(cursor.col > cursor.hang->cur_len){
            cursor.col = cursor.hang->cur_len;
        }
        else
        {
            cursor.col = 0;
            for(int i = 0; i < (n - 2 * cursor.chi_cnt); i++)
                cursor_right();
        }
        //MODIFIED

        cursor.row--;
    }
}
void Document::cursor_down()//MODIFIED
{
    qDebug() << "@Cursor down";
    int n = cursor.col;
    if(cursor.hang->Next_Row){
        cursor.hang = cursor.hang->Next_Row;
        if(cursor.col > cursor.hang->cur_len){
            cursor.col = cursor.hang->cur_len;
        }
        else
        {
            cursor.col = 0;
            for(int i = 0; i < (n - 2 * cursor.chi_cnt); i++)
                cursor_right();
        }
        //MODIFIED

        cursor.row++;
    }
}
void Document::cursor_home()
{
    qDebug() << "@Cursor home";
    cursor.hang = first_row;
    cursor.col = 0;
}
void Document::cursor_end()
{
    qDebug() << "@Cursor end";
    while(cursor.hang->Next_Row){
        cursor.hang = cursor.hang->Next_Row;
    }
    cursor.col = cursor.hang->cur_len;
}

void Document::clear_all()//MODIFIED
{
    isModified = false;
    Row *tmp = this->first_row, *tmp_next;
    while(tmp)
    {
        tmp_next = tmp->Next_Row;
        delete tmp;
        tmp = tmp_next;
    }

    //创建第一行
    this->first_row = new Row;
    this->cursor.hang = this->first_row;
    this->first_row->Prev_Row = NULL;
    //光标初始化
    cursor.hang = first_row;
    cursor.col = 0;
    cursor.row = 0;
    cursor.chi_cnt = 0;
    //文档逻辑
    isModified = false;
    file_name = "";

}

#include "document.h"
#include "mainwindow.h"

/********* Row *********/
Row::Row()
{
    qDebug() << " Row 构造函数";
    this->cur_len = 0;
    this->max_len = 100;
    this->next_row = NULL;
    this->prev_row = NULL;
    this->row_text = (char*)calloc(1, sizeof(char) * 100);
}

void Row::expand_block()
{
    qDebug() << "调用 expand_block()";
    this->row_text = (char*)realloc(row_text, this->max_len + 100 * sizeof(char));
    this->max_len += 100;
}


/******* Document *******/
Document::Document()
{
    qDebug() << " Document 构造函数";
    //创建第一行
    this->first_row = new Row;
    this->cursor.row_ptr = this->first_row;
    this->first_row->prev_row = NULL;
    //光标初始化
    cursor.row_ptr = first_row;
    cursor.col = 0;
    cursor.row_cnt = 0;
    cursor.chi_cnt = 0;
    //文档逻辑判断初始化
    isModified = false;
    file_name = "";
}

void Document::insert_text(const char *s)
{
    qDebug() << "***DOCUMNET*** 调用 insert_text()";
    this->isModified = true;
    int s_len = strlen(s);
    //检查是否需要分配空间
    while (s_len >= (this->cursor.row_ptr->max_len - this->cursor.row_ptr->cur_len))
    {
        qDebug() << "insert_text()循环";
        this->cursor.row_ptr->expand_block();
    }
    this->cursor.row_ptr->cur_len += s_len;
    //将当前行光标后字符后挪 s_len 长度
    for(int i = cursor.row_ptr->cur_len - 1; i >= cursor.col; i--)
    {
        while(cursor.row_ptr->cur_len + s_len > cursor.row_ptr->max_len)
           cursor.row_ptr->expand_block();
        cursor.row_ptr->row_text[i + s_len] = cursor.row_ptr->row_text[i];
    }
    //填入当前输入字符串
    for(int j = 0; j < s_len; j++)
        cursor.row_ptr->row_text[j + cursor.col] = s[j];
    //光标移动
    this->cursor.col += s_len;
}

void Document::new_row(Row *ptr)
{
    this->isModified = true;
    Row *tmp = new Row;
    tmp->prev_row = ptr;
    tmp->next_row = ptr->next_row;
    if(ptr->next_row)
        ptr->next_row->prev_row = tmp;
    ptr->next_row = tmp;
    ptr = tmp;
}

void Document::delete_row(Row *ptr)
{
    this->isModified = true;
    Row *tmp = new Row;
    tmp = ptr->prev_row;
    tmp->next_row = ptr->next_row;
    if(ptr->next_row)
        ptr->next_row->prev_row = tmp;
    delete ptr;
}

//读取文件到document对象，文件路径为char *s;
void Document::read_file(char *s)
{
    qDebug() << "调用 read_file 函数";
    QFile inputFile(s);
    QString line_text;
    string str;
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.setCodec(QTextCodec::codecForName("gbk"));
        //按行读入文档
        while(in.readLineInto(&line_text))
        {
            str = line_text.toStdString();
            this->insert_text((char*)str.c_str());
            this->new_row(this->cursor.row_ptr);
            this->cursor.cur_height++;
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
    if(outputFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&outputFile);
        out.setCodec(QTextCodec::codecForName("gbk"));
        while(temp){
            //按行保存到文件
            line_text = temp->row_text;
            line_text.toStdString();
            out << line_text;
            out << '\n';
            temp = temp->next_row;
        }
        outputFile.close();
        this->isModified = false;
        return true;
    }
    else
        return false;
}

void Document::cursor_left()
{
    qDebug() << "@Cursor Left";
    if(cursor.col == 0){
        if(cursor.row_ptr->prev_row) {
            qDebug() << "@@@@@@@@@@@@@@@" << cursor.row_ptr;
            qDebug() << "@@@@@@@@@@@@@@@" << cursor.row_ptr->prev_row;
            cursor.row_ptr = cursor.row_ptr->prev_row;
            qDebug() << "@@@@@@@@@@@@@@@" << cursor.row_ptr;
            qDebug() << "@@@@@@@@@@@@@@@" << cursor.row_ptr->prev_row;
            cursor.col = cursor.row_ptr->cur_len;
            cursor.row_cnt--;
        }
    }
    else{
        //判断中文字符(中文小于0)
        if(cursor.row_ptr->row_text[cursor.col - 1] > 0)
            cursor.col--;
        else{
            cursor.col -= 3;
        }
    }
}
void Document::cursor_right()
{
    qDebug() << "@Cursor Right";
    if(cursor.col >= cursor.row_ptr->cur_len){
        if(cursor.row_ptr->next_row){
            cursor.row_ptr = cursor.row_ptr->next_row;
            cursor.col = 0;
            cursor.row_cnt++;
        }
        else
            cursor.col = cursor.row_ptr->cur_len;
    }
    else{
        //判断中文字符(中文小于0)
        if(cursor.row_ptr->row_text[cursor.col] > 0)
            cursor.col++;
        else {
            cursor.col += 3;
        }
    }
}

void Document::cursor_up()
{
    qDebug() << "@Cursor up";
    int n = cursor.col;
    if(cursor.row_ptr->prev_row){
        cursor.row_ptr = cursor.row_ptr->prev_row;
        if(cursor.col > cursor.row_ptr->cur_len){
            cursor.col = cursor.row_ptr->cur_len;
        }
        else{
            cursor.col = 0;
            for(int i = 0; i < (n - 2 * cursor.chi_cnt); i++)
                cursor_right();
        }
        cursor.row_cnt--;
    }
}

void Document::cursor_down()
{
    qDebug() << "@Cursor down";
    int mov = cursor.col;
    if(cursor.row_ptr->next_row){
        cursor.row_ptr = cursor.row_ptr->next_row;
        if(cursor.col > cursor.row_ptr->cur_len){
            cursor.col = cursor.row_ptr->cur_len;
        }
        else{
            cursor.col = 0;
            for(int i = 0; i < (mov - 2 * cursor.chi_cnt); i++)
                cursor_right();
        }
        cursor.row_cnt++;
    }
}

void Document::cursor_home()
{
    qDebug() << "@Cursor home";
    cursor.col = 0;
}

void Document::cursor_end()
{
    qDebug() << "@Cursor end";
    cursor.col = cursor.row_ptr->cur_len;
}

void Document::clear_all()
{
    isModified = false;
    //删除每一行
    Row *tmp = this->first_row, *tmp_next;
    while(tmp){
        tmp_next = tmp->next_row;
        delete tmp;
        tmp = tmp_next;
    }
    //重建第一行
    this->first_row = new Row;
    this->cursor.row_ptr = this->first_row;
    //光标初始化
    cursor.row_ptr = first_row;
    cursor.col = 0;
    cursor.row_cnt = 0;
    cursor.chi_cnt = 0;
    //文档逻辑
    isModified = false;
    file_name = "";

}

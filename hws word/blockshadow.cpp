#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::add_blockshadow()//块选阴影
{
    Row *temprow = Doc->cursor.row_ptr;

    int sc=start_col;
    int ec=end_col;
    int size = block_label.size();
    if(size <= (start_row<=end_row?end_row:start_row))//如果块选阴影label数组大小小于最大行，则增大数组大小
    {
        for(int i = 0;i<=(start_row<=end_row?end_row:start_row) - size;i++)
        {
            QLabel *blocklabel = new QLabel(ui->scrollAreaWidgetContents_2);
            block_label.append(blocklabel);
        }
    }
    if(start_row == end_row)//如果只选中了一行
    {
        block_label[start_row]->setStyleSheet("background-color:rgba(198,198,198,148)");
        block_label[start_row]->resize(word_width*(fabs(end_col-start_col)-(fabs(chinese_judge(temprow,sc)-chinese_judge(temprow,ec)))*1.08),word_height);
        block_label[start_row]->move(word_width*((start_col>=end_col?end_col:start_col)-chinese_judge(temprow,sc>=ec?ec:sc)*1.08)-word_width,word_height*start_row);
        block_label[start_row]->show();
    }
    else
    {
        for(int i = 0;i < fabs(start_row - end_row);i++)
            if(start_row < end_row)
                temprow = temprow->prev_row;

        for(int i = start_row>=end_row?end_row:start_row;i<=(start_row<=end_row?end_row:start_row);i++)
        {
            int sc=start_col;
            int ec=end_col;
            int rowsize = temprow->cur_len;
            if(start_row < end_row)//如果是正向块选
            {
                qDebug() << start_row << end_row;
                if(i==start_row)
                {
                    qDebug() << "选中第一行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*(temprow->cur_len-start_col+1-(chinese_judge(temprow,rowsize)-chinese_judge(temprow,sc))*1.08),word_height);
                    block_label[i]->move(word_width*(start_col-chinese_judge(temprow,sc)*1.08)-word_width,word_height*i);
                    block_label[i]->show();
                }
                else if(i==end_row)
                {
                    qDebug() << "选中最后一行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*((end_col-1)-chinese_judge(temprow,ec)*1.08),word_height);
                    block_label[i]->move(0,word_height*i);
                    block_label[i]->show();
                }
                else
                {
                    qDebug() << "选中其他行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*(temprow->cur_len-chinese_judge(temprow,rowsize)*1.08),word_height);
                    block_label[i]->move(0,word_height*i);
                    block_label[i]->show();
                }
                qDebug() << temprow->cur_len;
                if(temprow)
                    temprow = temprow->next_row;
            }
            else if(start_row > end_row)//如果是逆向块选
            {
                if(i==start_row)
                {
                    qDebug() << "选中最后一行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*((start_col-1)-chinese_judge(temprow,sc)*1.08),word_height);
                    block_label[i]->move(0,word_height*i);
                    block_label[i]->show();
                }
                else if(i==end_row)
                {
                    qDebug() << "选中第一行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*(temprow->cur_len-end_col+1-(chinese_judge(temprow,rowsize)-chinese_judge(temprow,ec))*1.08),word_height);
                    block_label[i]->move(word_width*(end_col-chinese_judge(temprow,ec)*1.08)-word_width,word_height*i);
                    block_label[i]->show();
                }
                else
                {
                    qDebug() << "选中其他行";
                    block_label[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    block_label[i]->resize(word_width*(temprow->cur_len-chinese_judge(temprow,rowsize)*1.08),word_height);
                    block_label[i]->move(0,word_height*i);
                    block_label[i]->show();
                }
                qDebug() << temprow->cur_len;
                if(temprow)
                    temprow = temprow->next_row;
            }
        }
    }
    /*删除多余的块选阴影*/
    if(start_row < end_row)
        for(int i =1+end_row;i<block_label.size();i++)
            block_label[i]->hide();
    else if(start_row > end_row)
        for(int i =0;i<end_row;i++)
            block_label[i]->hide();
    else
    {
        for(int i =0;i < end_row;i++)
            block_label[i]->hide();
        for(int i =1+end_row;i<block_label.size();i++)
            block_label[i]->hide();
    }
    isSelectedblock = true;
}

void MainWindow::block_copy()//块拷贝
{
    if(!isSelectedblock || (start_col == end_col && start_row == end_row))
        return;
    QClipboard *board = QApplication::clipboard();
    QString newText;
    int num_string;
    if ( start_row == end_row ) /* 只在一行 */
    {
        num_string = fabs( start_col - end_col );
        char str[num_string];
        int num = 0;
        qDebug() << "##########" <<num_string;
        Doc->cursor.col	= start_col < end_col ? start_col - 1 : end_col - 1;
        while ( num_string-- )
        {
            str[num++] = Doc->cursor.row_ptr->row_text[Doc->cursor.col++];
        }
        str[num++] = '\0';
        newText = str;
        board->setText( newText );
    }else {
        if ( start_row < end_row ){/*正选*/
            int row_num = end_row - start_row;
            for(int i = 0; i<row_num; i++){
                Doc->cursor.row_ptr = Doc->cursor.row_ptr->prev_row;
            }
            Doc->cursor.col = start_col - 1;

            int tmp = start_col;
            start_col = end_col;
            end_col = tmp;

            tmp = start_row;
            start_row = end_row;
            end_row = tmp;
        }


        if(end_col < 1)
            end_col = 1;
        int temp = start_row - end_row;
        Row *row_temp = Doc->cursor.row_ptr;
        int str_len = 0;
        for(int i = temp; i > 0; i--){ //计算str长度(注意加上'\n'的个数)
            if( i == temp )
                str_len += row_temp->cur_len - end_col;
            row_temp = row_temp->next_row;
            if( i == 1 )
                str_len += start_col;
            else
                str_len += row_temp->cur_len;
            str_len++;
        }
        qDebug() << "#######  str_len:" << str_len;
        row_temp = Doc->cursor.row_ptr;
        char str[str_len + 1];
        int num = 0;
        for(int i = temp; i > 0; i--){//依次往下获取文字内容
            if(i == temp){
                for(int j = end_col - 1; j < row_temp->cur_len; j++){
                    str[num++] = row_temp->row_text[j];
                }
            }
            str[num++] = '\n';
            row_temp = row_temp->next_row;
            if(i == 1){
                for(int j = 0; j < start_col - 1; j++){
                    str[num++] = row_temp->row_text[j];
                }
            }else{
                for(int j = 0; j < row_temp->cur_len; j++)
                    str[num++] = row_temp->row_text[j];
            }
        }
        str[num++] = '\0';
        newText = str;
        board->setText(newText);
    }
}

void MainWindow::block_selectall()//全选
{
    start_row = 0;
    end_row = Doc->cursor.cur_height;
    start_col = 1;
    while(Doc->cursor.row_ptr->next_row)
        Doc->cursor_down();
    end_col = Doc->cursor.row_ptr->cur_len+1;
    Doc->cursor.col = end_col - 1;
    add_blockshadow();
    isSelectedblock = true;
}
void MainWindow::paste_text(QString clip_str)
{
    if(!clip_str.isEmpty()){
        string s = clip_str.toStdString();
        int tmp = 0, ch_cnt = 0;
        for(int i = Doc->cursor.col; i < Doc->cursor.row_ptr->cur_len; i++) {
            s += Doc->cursor.row_ptr->row_text[i];
            if (Doc->cursor.row_ptr->row_text[i] < 0)
                ch_cnt++;
            Doc->cursor.row_ptr->row_text[i] = '\0';
            tmp++;
        }
        Doc->cursor.row_ptr->cur_len -= tmp;
        //需要改粘贴多行的情况，分割"\n"
        char c = '\n';
        int mov = 0;
        string buff{""};
        vector <string> v;
        for (auto n:s) {
            if (n != c) buff += n;
            else if (n == c ) {
                mov++;
                v.push_back(buff);
                buff = "";
            }
        }
        if (buff != "") v.push_back(buff);
        for (auto n:v) {
            this->Doc->insert_text((char *) n.c_str());
            Doc->new_row(Doc->cursor.row_ptr);
            Doc->cursor_down();
            Doc->cursor.col = 0;
            Doc->cursor.cur_height++;
        }
        backspace_pressed();
        Doc->cursor_right();
        qDebug() << "^^^^^ tmp ^^^^^:" << tmp <<"chi_cnt:" << ch_cnt;
        for(int i = 0; i < tmp - (ch_cnt/3) * 2; i++)//
            Doc->cursor_left();

    }

}

void MainWindow::block_paste()//粘贴
{

    if(isSelectedblock)
        block_delete();
    QClipboard *board = QApplication::clipboard();
    QString clip_str = board->text();

    paste_text(clip_str);

    buffer_cache();
    //刷新
    refresh_screen();
}


void MainWindow::block_delete()//块删除
{
    int	num_str	= 0;
    int row_difference = fabs( start_row - end_row );
    int sc=start_col;
    int ec=end_col;
    if ( row_difference == 0 )//行差为0，即删除一行中的内容
    {
        num_str = fabs( start_col - end_col )-2*fabs(chinese_judge(Doc->cursor.row_ptr,sc)-chinese_judge(Doc->cursor.row_ptr,ec));
        Doc->cursor.col = start_col > end_col ? start_col - 1 : end_col - 1;
        while ( num_str-- )
        {
            int mov = 1;
            if ( Doc->cursor.row_ptr->row_text[Doc->cursor.col - 1] < 0 )
                mov = 3;
            for ( int i = Doc->cursor.col - mov; i <= Doc->cursor.row_ptr->cur_len - mov; i++ )
            {
                Doc->cursor.row_ptr->row_text[i]	= Doc->cursor.row_ptr->row_text[i + mov];
                Doc->cursor.row_ptr->row_text[i + mov] = '\0';
            }
            Doc->cursor.col -= mov;
            Doc->cursor.row_ptr->cur_len	-= mov;
            Doc->isModified = true;
        }
    } else if ( row_difference == 1 )//行差为1，即删除两行间的内容
    {
        if ( end_row < start_row )              /* 倒选 */
        {
            int rowsize = Doc->cursor.row_ptr->cur_len;
            num_str += (Doc->cursor.row_ptr->cur_len + 1 - end_col-2*(chinese_judge(Doc->cursor.row_ptr,rowsize)-chinese_judge(Doc->cursor.row_ptr,ec)));
            Doc->cursor.row_ptr	= Doc->cursor.row_ptr->next_row;
            num_str += start_col - 2*chinese_judge(Doc->cursor.row_ptr,sc);
            Doc->cursor.row_cnt	= start_row;
            Doc->cursor.col	= start_col - 1;
        } else {                                /* 正选 */
            num_str	+= (end_col - 2*chinese_judge(Doc->cursor.row_ptr,ec));
            int rowsize = Doc->cursor.row_ptr->prev_row->cur_len;
            num_str	+= (Doc->cursor.row_ptr->prev_row->cur_len + 1 - start_col-2*(chinese_judge(Doc->cursor.row_ptr->prev_row,rowsize)-chinese_judge(Doc->cursor.row_ptr->prev_row,sc)));
        }
        while ( num_str-- )
        {
            if ( Doc->cursor.col > 0 )  /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( Doc->cursor.row_ptr->row_text[Doc->cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = Doc->cursor.col - mov; i <= Doc->cursor.row_ptr->cur_len - mov; i++ )
                {
                    Doc->cursor.row_ptr->row_text[i]	= Doc->cursor.row_ptr->row_text[i + mov];
                    Doc->cursor.row_ptr->row_text[i + mov] = '\0';
                }
                Doc->cursor.col		-= mov;
                Doc->cursor.row_ptr->cur_len	-= mov;
                Doc->isModified		= true;
            } else if ( Doc->cursor.col == 0 &&
                    Doc->cursor.row_ptr != Doc->first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = Doc->cursor.row_ptr;
                Doc->cursor.row_ptr = temp->prev_row;
                if ( temp->next_row )
                {
                    temp->next_row->prev_row = Doc->cursor.row_ptr;
                }
                Doc->cursor.row_ptr->next_row	= temp->next_row;
                Doc->cursor.col		= Doc->cursor.row_ptr->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= Doc->cursor.col;
                while ( i < temp->cur_len )
                {
                    Doc->cursor.row_ptr->row_text[col++] = temp->row_text[i++];
                    Doc->cursor.row_ptr->cur_len++;
                }
                Doc->cursor.row_cnt--;
                Doc->cursor.cur_height--;
                delete temp;
                Doc->isModified = true;
            }
        }
    } else if ( row_difference > 1 )//行差大于1，即删除3行或3行以上内容
    {
        int temp = row_difference;
        if ( end_row < start_row ) /* 倒选 */
        {
            while ( temp > 1 )
            {
                Doc->delete_row( Doc->cursor.row_ptr->next_row );
                Doc->cursor.cur_height--;
                start_row--;
                temp--;
            }
            int rowsize = Doc->cursor.row_ptr->cur_len;
            num_str		+= (Doc->cursor.row_ptr->cur_len + 1 - end_col-2*(chinese_judge(Doc->cursor.row_ptr,rowsize)-chinese_judge(Doc->cursor.row_ptr,ec)));
            Doc->cursor.row_ptr	= Doc->cursor.row_ptr->next_row;
            num_str		+= start_col - 2*chinese_judge(Doc->cursor.row_ptr,sc);
            Doc->cursor.row_cnt	= start_row;
            Doc->cursor.col	= start_col - 1;
        } else { /* 正选 */
            int temp = row_difference;
            while ( temp > 1 )
            {
                Doc->delete_row( Doc->cursor.row_ptr->prev_row );
                Doc->cursor.cur_height--;
                end_row--;
                temp--;
            }
            num_str	+= (end_col - 2*chinese_judge(Doc->cursor.row_ptr,ec));
            int rowsize = Doc->cursor.row_ptr->prev_row->cur_len;
            num_str	+= (Doc->cursor.row_ptr->prev_row->cur_len + 1 - start_col-2*(chinese_judge(Doc->cursor.row_ptr->prev_row,rowsize)-chinese_judge(Doc->cursor.row_ptr->prev_row,sc)));
            Doc->cursor.row_cnt	= end_row;
            Doc->cursor.col	= end_col - 1;
        }
        while ( num_str-- )
        {
            if ( Doc->cursor.col > 0 ) /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( Doc->cursor.row_ptr->row_text[Doc->cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = Doc->cursor.col - mov; i <= Doc->cursor.row_ptr->cur_len - mov; i++ )
                {
                    Doc->cursor.row_ptr->row_text[i]	= Doc->cursor.row_ptr->row_text[i + mov];
                    Doc->cursor.row_ptr->row_text[i + mov] = '\0';
                }
                Doc->cursor.col		-= mov;
                Doc->cursor.row_ptr->cur_len	-= mov;
                Doc->isModified		= true;
            } else if ( Doc->cursor.col == 0 &&
                    Doc->cursor.row_ptr != Doc->first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = Doc->cursor.row_ptr;
                Doc->cursor.row_ptr = temp->prev_row;
                if ( temp->next_row )
                {
                    temp->next_row->prev_row = Doc->cursor.row_ptr;
                }
                Doc->cursor.row_ptr->next_row	= temp->next_row;
                Doc->cursor.col		= Doc->cursor.row_ptr->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= Doc->cursor.col;
                while ( i < temp->cur_len )
                {
                    Doc->cursor.row_ptr->row_text[col++] = temp->row_text[i++];
                    Doc->cursor.row_ptr->cur_len++;
                }
                Doc->cursor.row_cnt--;
                Doc->cursor.cur_height--;
                delete temp;
                Doc->isModified = true;
            }
        }
    }
    for ( int i = 0; i < block_label.size(); i++ )
        block_label[i]->hide();

    isSelectedblock = false;
    //start_x = start_y =end_x=end_y=end_col=end_row=start_col=start_row=-1;
    //refresh_screen();
}

void MainWindow::block_cut()
{
    buffer_cache();
    buffer_cache();
    block_copy();
    backspace_pressed();
    refresh_screen();
}

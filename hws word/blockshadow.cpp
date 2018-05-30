#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::add_blockshadow()
{
    Row *temprow = Doc.cursor.row_ptr;

    int size = test.size();
    if(size <= end_row)
    {
        for(int i = 0;i<=end_row - size;i++)
        {
            QLabel *blocklabel = new QLabel(ui->scrollAreaWidgetContents_2);
            test.append(blocklabel);
        }
    }
    if(start_row == end_row)
    {
        test[start_row]->setStyleSheet("background-color:rgba(198,198,198,148)");
        test[start_row]->resize(12*fabs(end_col-start_col),26);
        test[start_row]->move(12*(start_col>=end_col?end_col:start_col)-9,26*start_row);
        test[start_row]->show();
    }
    else
    {
        for(int i = 0;i < fabs(start_row - end_row);i++)
        {
            if(start_row < end_row)
            {
                qDebug() << "调用一次";
                temprow = temprow->prev_row;
            }

        }
        for(int i = start_row>=end_row?end_row:start_row;i<=(start_row<=end_row?end_row:start_row);i++)
        {
            if(start_row < end_row)
            {
                qDebug() << start_row << end_row;
                if(i==start_row)
                {
                    qDebug() << "选中第一行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*(temprow->cur_len-start_col+1),26);
                    test[i]->move(12*start_col-9,26*i);
                    test[i]->show();
                }
                else if(i==end_row)
                {
                    qDebug() << "选中最后一行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*(end_col-1),26);
                    test[i]->move(3,26*i);
                    test[i]->show();
                }
                else
                {
                    qDebug() << "选中其他行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*temprow->cur_len,26);
                    test[i]->move(3,26*i);
                    test[i]->show();
                }
                qDebug() << temprow->cur_len;
                if(temprow)
                    temprow = temprow->next_row;
            }
            else if(start_row > end_row)
            {
                if(i==start_row)
                {
                    qDebug() << "选中第一行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*(start_col-1),26);
                    test[i]->move(3,26*i);
                    test[i]->show();
                }
                else if(i==end_row)
                {
                    qDebug() << "选中最后一行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*(temprow->cur_len-end_col+1),26);
                    test[i]->move(12*end_col-9,26*i);
                    test[i]->show();
                }
                else
                {
                    qDebug() << "选中其他行";
                    test[i]->setStyleSheet("background-color:rgba(198,198,198,148)");
                    test[i]->resize(12*temprow->cur_len,26);
                    test[i]->move(3,26*i);
                    test[i]->show();
                }
                qDebug() << temprow->cur_len;
                if(temprow)
                    temprow = temprow->next_row;
            }
        }
    }
    if(start_row < end_row)
        for(int i =1+end_row;i<test.size();i++)
        {
            test[i]->hide();
        }
    else if(start_row > end_row)
        for(int i =0;i<end_row;i++)
        {
            test[i]->hide();
        }
    else
    {
        for(int i =0;i < end_row;i++)
        {
            test[i]->hide();
        }
        for(int i =1+end_row;i<test.size();i++)
        {
            test[i]->hide();
        }
    }
    isSelectedblock = true;
}
void MainWindow::block_copy()
{
    QClipboard	*board = QApplication::clipboard();
    QString		newText;
    int		num_string;
    if ( start_row == end_row ) /* 只在一行 */
    {
        num_string		= fabs( start_col - end_col );
        Doc.cursor.col	= start_col < end_col ? start_col - 1 : end_col - 1;
        while ( num_string-- )
        {
            newText.append( Doc.cursor.row_ptr->row_text[Doc.cursor.col++] );
        }
        board->setText( newText );
    }else if ( start_row < end_row ) /* 正选 */
    {
        int temp = end_row - start_row;
        for(int i=temp;i>0;i--)
            Doc.cursor.row_ptr=Doc.cursor.row_ptr->prev_row;
        Doc.cursor.row_cnt = start_row;
        Doc.cursor.col = start_col-1;
        while(Doc.cursor.col <=Doc.cursor.row_ptr->cur_len)
            newText.append(Doc.cursor.row_ptr->row_text[Doc.cursor.col++]);
        newText.append('\n');
        Doc.cursor.col = 0;
        Doc.cursor.row_ptr=Doc.cursor.row_ptr->next_row;
        for(int i=1;i<temp;i++)
        {
            newText.append(Doc.cursor.row_ptr->row_text);
            newText.append('\n');
            Doc.cursor.row_ptr=Doc.cursor.row_ptr->next_row;
        }
        while(Doc.cursor.col < end_col-1)
            newText.append(Doc.cursor.row_ptr->row_text[Doc.cursor.col++]);
        Doc.cursor.row_cnt = end_row;
        Doc.cursor.col = end_col-1;
        board->setText(newText);
    }else  { /* 倒选 */
        int temp = end_row - start_row;

        while(Doc.cursor.col <=Doc.cursor.row_ptr->cur_len)
            newText.append(Doc.cursor.row_ptr->row_text[Doc.cursor.col++]);
        newText.append('\n');
        Doc.cursor.col = 0;
        Doc.cursor.row_ptr=Doc.cursor.row_ptr->next_row;
        for(int i=1;i<temp;i++)
        {
            newText.append(Doc.cursor.row_ptr->row_text);
            newText.append('\n');
            Doc.cursor.row_ptr=Doc.cursor.row_ptr->next_row;
        }
        while(Doc.cursor.col < start_col-1)
            newText.append(Doc.cursor.row_ptr->row_text[Doc.cursor.col++]);
        for(int i=temp;i>0;i--)
            Doc.cursor.row_ptr=Doc.cursor.row_ptr->next_row;
        Doc.cursor.row_cnt = end_row;
        Doc.cursor.col = end_col-1;
        board->setText(newText);
    }
}


void MainWindow::block_paste()
{
    if(isSelectedblock)
        block_delete();
    qDebug() << "调用 BLOCK_PASTE";
    QClipboard *board = QApplication::clipboard();
    QString clip_str = board->text();
    string s = clip_str.toStdString();

    int tmp = 0, ch_cnt = 0;
    for(int i = Doc.cursor.col; i < Doc.cursor.row_ptr->cur_len; i++) {
        s += Doc.cursor.row_ptr->row_text[i];
        if (Doc.cursor.row_ptr->row_text[i] < 0)
            ch_cnt++;
        tmp++;
        Doc.cursor.row_ptr->row_text[i] = '\0';
    }
    Doc.cursor.row_ptr->cur_len -= tmp;

    //需要改粘贴多行的情况
    //'\n'的分开
    char c = '\n';
    string buff{""};
    vector <string> v;

    for (auto n:s) {
        if (n != c) buff += n;
        else if (n == c && buff != "") {
            v.push_back(buff);
            buff = "";
        }
    }
    if (buff != "") v.push_back(buff);
    for (auto n:v) {
        this->Doc.insert_text((char *) n.c_str());
        Doc.new_row(Doc.cursor.row_ptr);
        Doc.cursor_down();
        Doc.cursor.cur_height++;
    }
    Doc.delete_row(Doc.cursor.row_ptr);
    Doc.cursor.cur_height--;
    for(int i = 0; i <= tmp - (ch_cnt/3) * 2; i++){
        Doc.cursor_left();
    }

    //刷新
    refresh_screen();

}


void MainWindow::block_delete()
{
    qDebug() << "调用 BLOCK_DELETE";
    int	num_string	= 0;
    int	times		= fabs( start_row - end_row );
    if ( times == 0 )
    {
        num_string		= fabs( start_col - end_col );
        Doc.cursor.col	= start_col > end_col ? start_col - 1 : end_col - 1;
        while ( num_string-- )
        {
            int mov = 1;
            if ( Doc.cursor.row_ptr->row_text[Doc.cursor.col - 1] < 0 )
                mov = 3;
            for ( int i = Doc.cursor.col - mov; i <= Doc.cursor.row_ptr->cur_len - mov; i++ )
            {
                Doc.cursor.row_ptr->row_text[i]	= Doc.cursor.row_ptr->row_text[i + mov];
                Doc.cursor.row_ptr->row_text[i + mov] = '\0';
            }
            Doc.cursor.col		-= mov;
            Doc.cursor.row_ptr->cur_len	-= mov;
            Doc.isModified		= true;
        }
    } else if ( times == 1 )
    {
        if ( end_row < start_row )              /* 倒选 */
        {
            num_string		+= (Doc.cursor.row_ptr->cur_len + 1 - end_col);
            Doc.cursor.row_ptr	= Doc.cursor.row_ptr->next_row;
            num_string		+= start_col;
            Doc.cursor.row_cnt	= start_row;
            Doc.cursor.col	= start_col - 1;
        } else {                                /* 正选 */
            num_string	+= end_col;
            num_string	+= (Doc.cursor.row_ptr->prev_row->cur_len + 1 - start_col);
        }
        while ( num_string-- )
        {
            if ( Doc.cursor.col > 0 )  /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( Doc.cursor.row_ptr->row_text[Doc.cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = Doc.cursor.col - mov; i <= Doc.cursor.row_ptr->cur_len - mov; i++ )
                {
                    Doc.cursor.row_ptr->row_text[i]	= Doc.cursor.row_ptr->row_text[i + mov];
                    Doc.cursor.row_ptr->row_text[i + mov] = '\0';
                }
                Doc.cursor.col		-= mov;
                Doc.cursor.row_ptr->cur_len	-= mov;
                Doc.isModified		= true;
            } else if ( Doc.cursor.col == 0 &&
                    Doc.cursor.row_ptr != Doc.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = Doc.cursor.row_ptr;
                Doc.cursor.row_ptr = temp->prev_row;
                if ( temp->next_row )
                {
                    temp->next_row->prev_row = Doc.cursor.row_ptr;
                }
                Doc.cursor.row_ptr->next_row	= temp->next_row;
                Doc.cursor.col		= Doc.cursor.row_ptr->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= Doc.cursor.col;
                while ( i < temp->cur_len )
                {
                    Doc.cursor.row_ptr->row_text[col++] = temp->row_text[i++];
                    Doc.cursor.row_ptr->cur_len++;
                }
                Doc.cursor.row_cnt--;
                Doc.cursor.cur_height--;
                delete temp;
                Doc.isModified = true;
            }
        }
    } else if ( times > 1 )
    {
        int temp = times;
        if ( end_row < start_row ) /* 倒选 */
        {
            while ( temp > 1 )
            {
                Doc.delete_row( Doc.cursor.row_ptr->next_row );
                Doc.cursor.cur_height--;
                start_row--;
                temp--;
            }
            num_string		+= (Doc.cursor.row_ptr->cur_len + 1 - end_col);
            Doc.cursor.row_ptr	= Doc.cursor.row_ptr->next_row;
            num_string		+= start_col;
            Doc.cursor.row_cnt	= start_row;
            Doc.cursor.col	= start_col - 1;
        } else { /* 正选 */
            int temp = times;
            while ( temp > 1 )
            {
                Doc.delete_row( Doc.cursor.row_ptr->prev_row );
                Doc.cursor.cur_height--;
                end_row--;
                temp--;
            }
            num_string		+= end_col;
            num_string		+= (Doc.cursor.row_ptr->prev_row->cur_len + 1 - start_col);
            Doc.cursor.row_cnt	= end_row;
            Doc.cursor.col	= end_col - 1;
        }
        while ( num_string-- )
        {
            if ( Doc.cursor.col > 0 ) /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( Doc.cursor.row_ptr->row_text[Doc.cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = Doc.cursor.col - mov; i <= Doc.cursor.row_ptr->cur_len - mov; i++ )
                {
                    Doc.cursor.row_ptr->row_text[i]	= Doc.cursor.row_ptr->row_text[i + mov];
                    Doc.cursor.row_ptr->row_text[i + mov] = '\0';
                }
                Doc.cursor.col		-= mov;
                Doc.cursor.row_ptr->cur_len	-= mov;
                Doc.isModified		= true;
            } else if ( Doc.cursor.col == 0 &&
                    Doc.cursor.row_ptr != Doc.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = Doc.cursor.row_ptr;
                Doc.cursor.row_ptr = temp->prev_row;
                if ( temp->next_row )
                {
                    temp->next_row->prev_row = Doc.cursor.row_ptr;
                }
                Doc.cursor.row_ptr->next_row	= temp->next_row;
                Doc.cursor.col		= Doc.cursor.row_ptr->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= Doc.cursor.col;
                while ( i < temp->cur_len )
                {
                    Doc.cursor.row_ptr->row_text[col++] = temp->row_text[i++];
                    Doc.cursor.row_ptr->cur_len++;
                }
                Doc.cursor.row_cnt--;
                Doc.cursor.cur_height--;
                delete temp;
                Doc.isModified = true;
            }
        }
    }
    for ( int i = 0; i < test.size(); i++ )
    {
        test[i]->hide();
    }
    isSelectedblock = false;

    start_x = start_y =end_x=end_y=end_col=end_row=start_col=start_row=-1;
    refresh_screen();
}

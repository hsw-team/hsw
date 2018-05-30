#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::add_blockshadow()
{
    Row *temprow = sentence.cursor.hang;

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
                temprow = temprow->Prev_Row;
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
                    temprow = temprow->Next_Row;
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
                    temprow = temprow->Next_Row;
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
void MainWindow::Block_Copy()
{
    QClipboard	*board = QApplication::clipboard();
    QString		newText;
    int		num_string;
    if ( start_row == end_row ) /* 只在一行 */
    {
        num_string		= fabs( start_col - end_col );
        sentence.cursor.col	= start_col < end_col ? start_col - 1 : end_col - 1;
        while ( num_string-- )
        {
            newText.append( sentence.cursor.hang->row_text[sentence.cursor.col++] );
        }
        board->setText( newText );
    }else if ( start_row < end_row ) /* 正选 */
    {
        int temp = end_row - start_row;
        for(int i=temp;i>0;i--)
            sentence.cursor.hang=sentence.cursor.hang->Prev_Row;
        sentence.cursor.row = start_row;
        sentence.cursor.col = start_col-1;
        while(sentence.cursor.col <=sentence.cursor.hang->cur_len)
            newText.append(sentence.cursor.hang->row_text[sentence.cursor.col++]);
        newText.append('\n');
        sentence.cursor.col = 0;
        sentence.cursor.hang=sentence.cursor.hang->Next_Row;
        for(int i=1;i<temp;i++)
        {
            newText.append(sentence.cursor.hang->row_text);
            newText.append('\n');
            sentence.cursor.hang=sentence.cursor.hang->Next_Row;
        }
        while(sentence.cursor.col < end_col-1)
            newText.append(sentence.cursor.hang->row_text[sentence.cursor.col++]);
        sentence.cursor.row = end_row;
        sentence.cursor.col = end_col-1;
        board->setText(newText);
    }else  { /* 倒选 */
        int temp = end_row - start_row;

        while(sentence.cursor.col <=sentence.cursor.hang->cur_len)
            newText.append(sentence.cursor.hang->row_text[sentence.cursor.col++]);
        newText.append('\n');
        sentence.cursor.col = 0;
        sentence.cursor.hang=sentence.cursor.hang->Next_Row;
        for(int i=1;i<temp;i++)
        {
            newText.append(sentence.cursor.hang->row_text);
            newText.append('\n');
            sentence.cursor.hang=sentence.cursor.hang->Next_Row;
        }
        while(sentence.cursor.col < start_col-1)
            newText.append(sentence.cursor.hang->row_text[sentence.cursor.col++]);
        for(int i=temp;i>0;i--)
            sentence.cursor.hang=sentence.cursor.hang->Next_Row;
        sentence.cursor.row = end_row;
        sentence.cursor.col = end_col-1;
        board->setText(newText);
    }
}


void MainWindow::Block_Paste()
{
    if(isSelectedblock)
        Block_Delete();
    qDebug() << "调用 BLOCK_PASTE";
    QClipboard *board = QApplication::clipboard();
    QString clip_str = board->text();
    string s = clip_str.toStdString();

    int tmp = 0, ch_cnt = 0;
    for(int i = sentence.cursor.col; i < sentence.cursor.hang->cur_len; i++) {
        s += sentence.cursor.hang->row_text[i];
        if (sentence.cursor.hang->row_text[i] < 0)
            ch_cnt++;
        tmp++;
        sentence.cursor.hang->row_text[i] = '\0';
    }
    sentence.cursor.hang->cur_len -= tmp;

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
        this->sentence.edit((char *) n.c_str());
        sentence.add_row(sentence.cursor.hang);
        sentence.cursor_down();
        sentence.cursor.cur_height++;
    }
    sentence.delete_row(sentence.cursor.hang);
    sentence.cursor.cur_height--;
    for(int i = 0; i <= tmp - (ch_cnt/3) * 2; i++){
        sentence.cursor_left();
    }

    //刷新
    refresh();

}


void MainWindow::Block_Delete()
{
    qDebug() << "调用 BLOCK_DELETE";
    int	num_string	= 0;
    int	times		= fabs( start_row - end_row );
    if ( times == 0 )
    {
        num_string		= fabs( start_col - end_col );
        sentence.cursor.col	= start_col > end_col ? start_col - 1 : end_col - 1;
        while ( num_string-- )
        {
            int mov = 1;
            if ( sentence.cursor.hang->row_text[sentence.cursor.col - 1] < 0 )
                mov = 3;
            for ( int i = sentence.cursor.col - mov; i <= sentence.cursor.hang->cur_len - mov; i++ )
            {
                sentence.cursor.hang->row_text[i]	= sentence.cursor.hang->row_text[i + mov];
                sentence.cursor.hang->row_text[i + mov] = '\0';
            }
            sentence.cursor.col		-= mov;
            sentence.cursor.hang->cur_len	-= mov;
            sentence.isModified		= true;
        }
    } else if ( times == 1 )
    {
        if ( end_row < start_row )              /* 倒选 */
        {
            num_string		+= (sentence.cursor.hang->cur_len + 1 - end_col);
            sentence.cursor.hang	= sentence.cursor.hang->Next_Row;
            num_string		+= start_col;
            sentence.cursor.row	= start_row;
            sentence.cursor.col	= start_col - 1;
        } else {                                /* 正选 */
            num_string	+= end_col;
            num_string	+= (sentence.cursor.hang->Prev_Row->cur_len + 1 - start_col);
        }
        while ( num_string-- )
        {
            if ( sentence.cursor.col > 0 )  /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( sentence.cursor.hang->row_text[sentence.cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = sentence.cursor.col - mov; i <= sentence.cursor.hang->cur_len - mov; i++ )
                {
                    sentence.cursor.hang->row_text[i]	= sentence.cursor.hang->row_text[i + mov];
                    sentence.cursor.hang->row_text[i + mov] = '\0';
                }
                sentence.cursor.col		-= mov;
                sentence.cursor.hang->cur_len	-= mov;
                sentence.isModified		= true;
            } else if ( sentence.cursor.col == 0 &&
                    sentence.cursor.hang != sentence.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = sentence.cursor.hang;
                sentence.cursor.hang = temp->Prev_Row;
                if ( temp->Next_Row )
                {
                    temp->Next_Row->Prev_Row = sentence.cursor.hang;
                }
                sentence.cursor.hang->Next_Row	= temp->Next_Row;
                sentence.cursor.col		= sentence.cursor.hang->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= sentence.cursor.col;
                while ( i < temp->cur_len )
                {
                    sentence.cursor.hang->row_text[col++] = temp->row_text[i++];
                    sentence.cursor.hang->cur_len++;
                }
                sentence.cursor.row--;
                sentence.cursor.cur_height--;
                delete temp;
                sentence.isModified = true;
            }
        }
    } else if ( times > 1 )
    {
        int temp = times;
        if ( end_row < start_row ) /* 倒选 */
        {
            while ( temp > 1 )
            {
                sentence.delete_row( sentence.cursor.hang->Next_Row );
                sentence.cursor.cur_height--;
                start_row--;
                temp--;
            }
            num_string		+= (sentence.cursor.hang->cur_len + 1 - end_col);
            sentence.cursor.hang	= sentence.cursor.hang->Next_Row;
            num_string		+= start_col;
            sentence.cursor.row	= start_row;
            sentence.cursor.col	= start_col - 1;
        } else { /* 正选 */
            int temp = times;
            while ( temp > 1 )
            {
                sentence.delete_row( sentence.cursor.hang->Prev_Row );
                sentence.cursor.cur_height--;
                end_row--;
                temp--;
            }
            num_string		+= end_col;
            num_string		+= (sentence.cursor.hang->Prev_Row->cur_len + 1 - start_col);
            sentence.cursor.row	= end_row;
            sentence.cursor.col	= end_col - 1;
        }
        while ( num_string-- )
        {
            if ( sentence.cursor.col > 0 ) /* 当指针不在行首的时候(之前有字符可以删除) */
            {
                /* 判断中文字符 */
                int mov = 1;
                if ( sentence.cursor.hang->row_text[sentence.cursor.col - 1] < 0 )
                    mov = 3;
                for ( int i = sentence.cursor.col - mov; i <= sentence.cursor.hang->cur_len - mov; i++ )
                {
                    sentence.cursor.hang->row_text[i]	= sentence.cursor.hang->row_text[i + mov];
                    sentence.cursor.hang->row_text[i + mov] = '\0';
                }
                sentence.cursor.col		-= mov;
                sentence.cursor.hang->cur_len	-= mov;
                sentence.isModified		= true;
            } else if ( sentence.cursor.col == 0 &&
                    sentence.cursor.hang != sentence.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
            {
                Row *temp = sentence.cursor.hang;
                sentence.cursor.hang = temp->Prev_Row;
                if ( temp->Next_Row )
                {
                    temp->Next_Row->Prev_Row = sentence.cursor.hang;
                }
                sentence.cursor.hang->Next_Row	= temp->Next_Row;
                sentence.cursor.col		= sentence.cursor.hang->cur_len;
                /* 需要把之后行的加到后面 */
                int	i	= 0;
                int	col	= sentence.cursor.col;
                while ( i < temp->cur_len )
                {
                    sentence.cursor.hang->row_text[col++] = temp->row_text[i++];
                    sentence.cursor.hang->cur_len++;
                }
                sentence.cursor.row--;
                sentence.cursor.cur_height--;
                delete temp;
                sentence.isModified = true;
            }
        }
    }
    for ( int i = 0; i < test.size(); i++ )
    {
        test[i]->hide();
    }
    isSelectedblock = false;

    start_x = start_y =end_x=end_y=end_col=end_row=start_col=start_row=-1;
    refresh();
}

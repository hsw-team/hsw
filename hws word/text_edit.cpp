#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::not_found_window(){
    QMessageBox box(QMessageBox::Question,"查找", "无此内容!");
    box.setIcon(QMessageBox::Information);
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("OK"));
    box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
    box.exec();
}

void MainWindow::find_window()//查找窗口
{
    found = false;
    notExist = false;
    qDebug() << "\n打开查找窗口\n" << endl;

    f.show();
}

void MainWindow::find(QString text)//查找
{
    qDebug() << "\n查找功能可用" << endl;
    qDebug() << "text = " << text << endl;
    Row *cur_row = Doc->cursor.row_ptr; // 定位查找起始行
    int col = Doc->cursor.col - Doc->cursor.chi_cnt * 2; //查找起始位置
    int pos = -1; //查找位置，初始化为-1
    QString words = "";

    if(cur_row == NULL){
        notExist = true;
        //添加弹窗
        not_found_window();
    }
    Q_ASSERT(cur_row != NULL);
    if(cur_row != NULL){
        words.append(cur_row->row_text);//将本行文本存入words
        if(found == false)//如果当前位置并非之前已经查找成功的位置
            pos = words.indexOf(text, col); //从col位置开始查找text
        else if(col + text.length() > words.length() - 1) //剩余长度不足以容纳查找词
            pos = -1;
        else{
            pos = words.indexOf(text, col + text.length()); //跳过当前查找成功词查找
        }

        int row_to_plus = 0;
        while(cur_row != NULL && pos == -1){
            found = false;
            cur_row = cur_row->next_row;
            if(cur_row == NULL)
                break;
            col = 0;
            words = "";
            words.append(cur_row->row_text);
            qDebug() << "words are:\n" << words << endl;
            row_to_plus++;

            pos = words.indexOf(text, col);
        }

        if(cur_row == NULL){
            qDebug() << "not found" << endl;
            notExist = true;
            not_found_window();//打开弹窗
        }

        else if(pos != -1){
            qDebug() << "THE FOUND! pos is" << pos << endl;
            found = true;
            Doc->cursor.row_ptr = cur_row;
            Q_ASSERT(cur_row != NULL);
            Doc->cursor.row_cnt += row_to_plus;

            // pos = 中文字数+英文字数
            // true_length = 中文字数*3 + 英文字数
            int true_length = pos; // true_length 是实际存储长度
            int i = 0;
            for(int count = 0; count < pos && i < Doc->cursor.row_ptr->cur_len; count++){
                if(Doc->cursor.row_ptr->row_text[i] < 0){
                    true_length += 2;
                    i += 2;
                }
                i++;
            }

          //  qDebug() << "pos before:" << pos << endl;
            Doc->cursor.col = true_length;
            chinese_count();
            english_count();
           // qDebug() << "pos after:" << pos << endl;

            qDebug() << "true_length is:" << Doc->cursor.col << endl;
            qDebug() << "CHINESE:" << Doc->cursor.chi_cnt << endl;
            col = pos; // 更新col，col是当前光标前的字数（无论中文英文）
            qDebug() << "after finding the col is" << col << endl;
            ui->label_cursor->setGeometry(word_width * (Doc->cursor.col - Doc->cursor.chi_cnt * 1.08) - 15,
                                                  word_height * Doc->cursor.row_cnt, 30, 30 );

            int length = text.length();
            int block_length = length;
            for(int i = 0; i < length; i++){
                QChar cha = text.at(i);
                ushort uni = cha.unicode();
                if(uni >= 0x4E00 && uni <= 0x9FA5)
                {
                    //这个字符是中文
                    block_length+= 2;
                }
            }

            refresh_screen();
            start_col = Doc->cursor.col + 1;
            start_row = Doc->cursor.row_cnt;
            end_col = start_col + block_length;
            end_row = start_row;

            add_blockshadow();
            isSelectedblock = true;
        }
    }
}

// 打开替换窗口
void MainWindow:: replace_window(){
    isFirstReplace = true;
    notExist = false;
    r.exec();
}

// 替换
void MainWindow:: replace(QString target, QString value){
    // target: 查找文本
    // value: 应替换的文本

    // 如果是第一次替换，并且当前不是被被Find Next查找选中的状态
    if(isFirstReplace && !isSelectedblock){
        found = false;
        notExist = false;
        find(target);//执行查找操作
        isFirstReplace = false;

        qDebug() << "第一次替换"  << endl;
    }
    else if(!notExist){//已选中当前查找的内容
        //先删除target
        qDebug() << "不是第一次替换" << endl;
        if(isSelectedblock){
            block_delete();
            isSelectedblock = false;
            found = false; // 替换后当前为未查找成功状态

            // 加上value
            std::string words = value.toStdString();
            const char* s = words.c_str();
            Doc->insert_text(s);
        }
        refresh_screen();

        //查找下一个
        find(target);


    }
    else{//已经没有内容，notExist = true
        qDebug() << " solved bug" << endl;

        //添加弹窗
        not_found_window();
    }

}

void MainWindow::replace_all(QString target, QString value)//全部替换
{

    while(!notExist){
        replace(target, value);
    }

    notExist = false;

}

void MainWindow::buffer_cache()
{
    qDebug() << "！！！！！!!!!!!!!!！！！调用了BUFFER_CACHE！！！！!!!!!!!！！!!！！！！！！";
    if(Undo.size() == 100){
        Undo.takeFirst();
    }
    if(Redo.size() == 100){
        Redo.takeFirst();
    }
    Document *cache = new Document;
    cache->file_name = Doc->file_name;
    cache->isModified = true;

    Row *tmp2 = Doc->first_row;
    while(tmp2){
        cache->insert_text(tmp2->row_text);
        cache->new_row(cache->cursor.row_ptr);
        cache->cursor.row_ptr = cache->cursor.row_ptr->next_row;
        cache->cursor.col = 0;
        tmp2 = tmp2->next_row;
    }
    cache->delete_row(cache->cursor.row_ptr);
    cache->cursor.row_ptr = cache->first_row;
    for(int i = 0; i < Doc->cursor.cur_height; i++){
        cache->cursor.row_ptr = cache->cursor.row_ptr->next_row;
    }
    cache->cursor.cur_height = Doc->cursor.cur_height;
    cache->cursor.col = Doc->cursor.col;
    cache->cursor.row_cnt = Doc->cursor.row_cnt;

    Undo.push(cache);

    while(!Redo.isEmpty()){
        Redo.pop();
    }

    qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
    qDebug() << "Undo count:" << Undo.size() << "Redo count:" << Redo.size();
    qDebug() << "Undo 位置和第一行内容:";
    for(auto n:Undo){
        qDebug() << n->first_row;
        qDebug() << n->first_row->row_text;
    }
    qDebug() << "Redo 位置和第一行内容:";
    for(auto n:Redo){
        qDebug() << n->first_row;
        qDebug() << n->first_row->row_text;
    }
    qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

}

void MainWindow::undo()
{
    qDebug() << "*** undo ***";
    if(Redo.isEmpty()&&(!Undo.isEmpty())){
        Undo.pop();
    }

    if(!Undo.isEmpty()){
        //Undo.pop();
        Redo.push(Doc);
        Doc = Undo.pop();
    }
    qDebug() << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
    qDebug() << Doc->cursor.row_ptr->row_text;

    refresh_screen();
}

void MainWindow::redo()
{
    qDebug() << "*** redo ***";

    if(!Redo.isEmpty()){
        Undo.push(Doc);
        Doc = Redo.pop();
    }

    refresh_screen();
}

void MainWindow::chinese_count()//计算当前光标之前的中文字符个数
{
    //qDebug() << "运行了chinese_count";
    Doc->cursor.chi_cnt = chinese_judge(Doc->cursor.row_ptr,Doc->cursor.col);
}

void MainWindow::english_count()//计算当前光标之前的英文字符个数
{
    Doc->cursor.eng_cnt = english_judge(Doc->cursor.row_ptr,Doc->cursor.col);
}

int MainWindow::chinese_judge(Row *row_ptr, int &col)
{
    //自动加减得到Cursor.col考虑中文应该的位置
    //返回的是Cursor.col之前中文的个数
    //qDebug() << "运行了chinese_judge";
    int chi_cnt = 0;
    for ( int i = 0; i < col; i++ )
        if ( row_ptr->row_text[i] < 0 )
            chi_cnt++;
    int n = chi_cnt % 3;
    for(;n && col; n--, col--);
    return chi_cnt/3;
}

int MainWindow::english_judge(Row *row_ptr, int col)
{
    int eng_cnt = 0;
    for(int i = 0; i < col; i++ )
        if(row_ptr->row_text[i] >= 0)
            eng_cnt++;
    return eng_cnt;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if ( object == this &&event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            // Special tab handling
            string n = "    ";
            Doc->insert_text( (char*)n.c_str() );
            qDebug() << "555555555555555555555555555555555555555555555555555555555555";
            buffer_cache();
            refresh_screen();
            return true;
        } else
            return false;
    }
    return false;
}

void MainWindow::enter_pressed()//按下回车
{
    if(isSelectedblock)
        block_delete();
    int	i, j = 0;
    int	col	= Doc->cursor.col;
    Row	*hang	= Doc->cursor.row_ptr;
    Doc->new_row( Doc->cursor.row_ptr );
    Doc->cursor.row_ptr = Doc->cursor.row_ptr->next_row;
    Doc->cursor.cur_height++;
    for ( i = col; i < hang->cur_len; i++ )
    {
        Doc->cursor.row_ptr->row_text[j++]	= hang->row_text[i];
        hang->row_text[i] = '\0';
    }
    hang->cur_len = col;
    Doc->cursor.row_ptr->cur_len	= j;
    Doc->cursor.col = 0;
    Doc->cursor.row_cnt++;
    Doc->isModified = true;
}

void MainWindow::delete_pressed()//按下delete
{
    if(isSelectedblock)
        block_delete();
    else if ( Doc->cursor.col < Doc->cursor.row_ptr->cur_len )  //当指针不在行尾的时候(之前有字符可以删除)
    {
        // 判断中文字符
        int mov = 1;
        if ( Doc->cursor.row_ptr->row_text[Doc->cursor.col] < 0 )
            mov = 3;

        for ( int i = Doc->cursor.col; i <= Doc->cursor.row_ptr->cur_len - mov; i++ )
        {
            Doc->cursor.row_ptr->row_text[i]	= Doc->cursor.row_ptr->row_text[i + mov];
            Doc->cursor.row_ptr->row_text[i + mov] = '\0';
        }
        Doc->cursor.row_ptr->cur_len	-= mov;
        Doc->isModified = true;
    }
    else if ( Doc->cursor.col == Doc->cursor.row_ptr->cur_len && Doc->cursor.row_ptr->next_row )
    {// 当在行尾，并且不是最后一行一行(最后一行行尾delete就不用操作了)
        Row *temp = Doc->cursor.row_ptr->next_row;
        if ( temp->next_row )
        {
            Doc->cursor.row_ptr->next_row = temp->next_row;
        } else {
            Doc->cursor.row_ptr->next_row = NULL;
        }
        // 需要把之后行的加到后面
        int	i	= 0;
        int	col	= Doc->cursor.col;
        while ( i < temp->cur_len )
        {
            Doc->cursor.row_ptr->row_text[col++] = temp->row_text[i++];
            Doc->cursor.row_ptr->cur_len++;
        }
        delete temp;
        Doc->cursor.cur_height--;
        Doc->isModified = true;
    }

}

void MainWindow::backspace_pressed()//按下backspace
{
    if ( isSelectedblock )
        block_delete();
    else if ( Doc->cursor.col > 0 )  //当指针不在行首的时候(之前有字符可以删除)
    {
        //判断中文字符
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
    else if ( Doc->cursor.col == 0 && Doc->cursor.row_ptr != Doc->first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
    {
        Row *temp = Doc->cursor.row_ptr;
        Doc->cursor.row_ptr = temp->prev_row;
        if ( temp->next_row )
        {
            temp->next_row->prev_row = Doc->cursor.row_ptr;
        }
        Doc->cursor.row_ptr->next_row = temp->next_row;
        Doc->cursor.col = Doc->cursor.row_ptr->cur_len;
        // 需要把之后行的加到后面
        int	i = 0;
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

void MainWindow::keyPressEvent( QKeyEvent *ev )//键盘事件
{
    this->setFocus();
    string n;
    n = ev->key();
    if ( ev->key() == Qt::Key_CapsLock ){
        if ( caps == true )
            caps = false;
        else
            caps = true;
    }
    else if ( ev->key() == Qt::Key_Left && ev->modifiers() == Qt::ShiftModifier)
    {
        if(!isSelectedblock)
        {
            english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            start_col = Doc->cursor.col + 1;
            start_row = Doc->cursor.row_cnt;
        }
        Doc->cursor_left();
        english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        end_col = Doc->cursor.col + 1;
        end_row = Doc->cursor.row_cnt;
        add_blockshadow();
        refresh_screen();
    }
    else if ( ev->key() == Qt::Key_Left ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_left();
    }

    else if ( ev->key() == Qt::Key_Up && ev->modifiers() == Qt::ShiftModifier)
    {
        if(!isSelectedblock)
        {
            english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            start_col = Doc->cursor.col + 1;
            start_row = Doc->cursor.row_cnt;
        }
        Doc->cursor_up();
        english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        end_col = Doc->cursor.col + 1;
        end_row = Doc->cursor.row_cnt;
        add_blockshadow();
        refresh_screen();
    }
    else if ( ev->key() == Qt::Key_Up ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_up();
    }
    else if ( ev->key() == Qt::Key_Right && ev->modifiers() == Qt::ShiftModifier)
    {
        if(!isSelectedblock)
        {
            english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            start_col = Doc->cursor.col + 1;
            start_row = Doc->cursor.row_cnt;
        }
        Doc->cursor_right();
        english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        end_col = Doc->cursor.col + 1;
        end_row = Doc->cursor.row_cnt;
        add_blockshadow();
        refresh_screen();
    }
    else if ( ev->key() == Qt::Key_Right ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_right();
    }
    else if ( ev->key() == Qt::Key_Down && ev->modifiers() == Qt::ShiftModifier)
    {
        if(!isSelectedblock)
        {
            english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
            start_col = Doc->cursor.col + 1;
            start_row = Doc->cursor.row_cnt;
        }
        Doc->cursor_down();
        english_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        chinese_judge(Doc->cursor.row_ptr, Doc->cursor.col);
        end_col = Doc->cursor.col + 1;
        end_row = Doc->cursor.row_cnt;
        add_blockshadow();
        refresh_screen();
    }
    else if ( ev->key() == Qt::Key_Down ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_down();
    }

    else if( ev->key() == Qt::Key_Home ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_home();
    }
    else if( ev->key() == Qt::Key_End ){
        isSelectedblock = false;
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        Doc->cursor_end();
    }
    else if( ev->key() == Qt::Key_PageUp){
        int page = ui->scrollArea->height()/word_height+1;
        int prev_col = Doc->cursor.col;
        while(page--)
            Doc->cursor_up();
        for(int i = 0; i < prev_col; i++)
            if(Doc->cursor.col < prev_col)
                Doc->cursor_right();
    }
    else if( ev->key() == Qt::Key_PageDown){
        int page = ui->scrollArea->height()/word_height+1;
        int prev_col = Doc->cursor.col;
        while(page--)
            Doc->cursor_down();
        for(int i = 0; i < prev_col; i++)
            if(Doc->cursor.col < prev_col)
                Doc->cursor_right();
    }
    else if( ev->key() == Qt::Key_Shift || ev->key() == Qt::Key_Control ||
             ev->key() == Qt::Key_Alt || ev->key() == Qt::Key_Insert ){
        ;
    }
    else if( (ev->key() <= Qt::Key_Z && ev->key() >= Qt::Key_A) && ev->modifiers() == Qt::ControlModifier ){
        ;
    }
    else if ( (ev->key() <= Qt::Key_Z && ev->key() >= Qt::Key_A) && ev->modifiers() == Qt::ShiftModifier ){
        if(isSelectedblock)
            block_delete();
        if ( caps == true )
        {
            transform( n.begin(), n.end(), n.begin(), ::tolower );
            Doc->insert_text( (char*)n.c_str() );
        } else {
            transform( n.begin(), n.end(), n.begin(), ::toupper );
            Doc->insert_text( (char*)n.c_str() );
        }
        buffer_cache();
    }
    else if ( ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return ){
        enter_pressed();
        buffer_cache();
    }
    else if ( ev->key() == Qt::Key_Backspace ) {
        int col_tmp = Doc->cursor.col;
        Row* row_tmp = Doc->cursor.row_ptr;
        backspace_pressed();
        if(col_tmp != Doc->cursor.col||row_tmp != Doc->cursor.row_ptr)
            buffer_cache();
    }
    else if ( ev->key() == Qt::Key_Delete ) {
        int col_tmp = Doc->cursor.col;
        Row* row_tmp = Doc->cursor.row_ptr;
        delete_pressed();
        if(col_tmp != Doc->cursor.col||row_tmp != Doc->cursor.row_ptr)
            buffer_cache();
    }
    else { // 英文输入
        if(isSelectedblock)
            block_delete();
        if ( caps == false )
            transform( n.begin(), n.end(), n.begin(), ::tolower );
        Doc->insert_text((char*)n.c_str());
        buffer_cache();
    }
    refresh_screen();
}

void MainWindow::inputMethodEvent( QInputMethodEvent *a )//获取中文输入
{
    qDebug() << "当前输入为" << a->commitString().toUtf8().data();
    qDebug() << Doc->cursor.chi_cnt;
    qDebug() << Doc->cursor.col;
    if(isSelectedblock)
        block_delete();
    Doc->insert_text( a->commitString().toUtf8().data() );
    this->setFocus();
    qDebug() << "11111111111111111111111111111111111111111111111";
    if(!a->commitString().toUtf8().isEmpty())
        buffer_cache();
    refresh_screen();
}

void MainWindow::mouseMoveEvent( QMouseEvent *event )//鼠标移动事件
{
    if ( event->buttons() & Qt::LeftButton )//如果按下左键
    {
        if ( start_x == -1 && start_y == -1 )//鼠标刚开始移动时，获取其坐标
        {
            start_x = event->pos().x();
            start_y = event->pos().y();

            pos_to_coordinate( start_x, start_y, false );
            start_col	= Doc->cursor.col + 1;
            /*遇到中文时，修正坐标*/
            int chinese_pos = start_col - Doc->cursor.eng_cnt - Doc->cursor.chi_cnt*3;
            while(chinese_pos % 3 != 1)
            {
                chinese_pos++;
                start_col++;
            }
            start_row	= Doc->cursor.row_cnt;
        }
        /*实时获取移动时的坐标*/
        end_x	= event->pos().x();
        end_y	= event->pos().y();

        pos_to_coordinate( end_x, end_y, false );
        end_col = Doc->cursor.col + 1;
        /*遇到中文时，修正坐标*/
        int chinese_pos = end_col - Doc->cursor.eng_cnt - Doc->cursor.chi_cnt*3;
        while(chinese_pos % 3 != 1)
        {
            chinese_pos++;
            end_col++;
        }
        end_row = Doc->cursor.row_cnt;

        qDebug() << "开始位置" << start_col << start_row;
        qDebug() << "结束位置" << end_col << end_row;
        add_blockshadow();
        refresh_screen();
    }
}

void MainWindow::mouseReleaseEvent( QMouseEvent *event )//鼠标释放事件
{
    if ( event->button() == Qt::LeftButton )
    {
        //重置起始坐标
        start_x		= -1;
        start_y		= -1;
        refresh_screen();
    }
}

void MainWindow::mousePressEvent( QMouseEvent *event )//鼠标点击事件
{
    /* 如果是鼠标左键按下   改变指针形状，并且存储当前指针位置与窗口位置的差值.Hello Please Tina */
    if ( event->button() == Qt::LeftButton )
    {
        for ( int i = 0; i < block_label.size(); i++ )
            block_label[i]->hide();
        isSelectedblock = false;
        pos_to_coordinate( event->pos().x(), event->pos().y(), true );
        refresh_screen();
    }
    /* 如果是鼠标右键按下 */
    else if ( event->button() == Qt::RightButton )
    {

    }
    this->setFocus();
}

void MainWindow::pos_to_coordinate( int x, int y, bool flag )//将坐标转换为对应行数和列数，并可选择是否要立即更新光标位置
{
    int	toolbar_height	= ui->mainToolBar->height() + ui->menuBar->height();
    int	hbar_height	= ui->scrollArea->horizontalScrollBar()->value();
    int	vbar_height	= ui->scrollArea->verticalScrollBar()->value();

    //y小于等于最大行数
    for ( int j = 0; j < Doc->cursor.cur_height; j++ )
    {
        if ( y + vbar_height > word_height * j + toolbar_height && y + vbar_height < word_height * (j + 1) + toolbar_height )
        {

            if ( Doc->cursor.row_cnt < j )
                for ( int temp = Doc->cursor.row_cnt; temp < j; temp++ )
                    Doc->cursor.row_ptr = Doc->cursor.row_ptr->next_row;
            else if ( Doc->cursor.row_cnt >= j )
                for ( int temp = 0; temp < Doc->cursor.row_cnt - j; temp++ )
                    Doc->cursor.row_ptr = Doc->cursor.row_ptr->prev_row;
            Doc->cursor.row_cnt = j;

            qDebug() << "Doc->cursor.row_cnt" << Doc->cursor.row_cnt;
            qDebug() << "Doc->cursor.col" << Doc->cursor.col;


            for ( int i = 0; i <= Doc->cursor.row_ptr->cur_len; i++ )
            {
                int j = i;
                Doc->cursor.chi_cnt = chinese_judge(Doc->cursor.row_ptr,j);

                if ( x + hbar_height > (word_width * (i-Doc->cursor.chi_cnt * 1.08) - 15) && x + hbar_height <= (word_width * (i-Doc->cursor.chi_cnt * 1.08 + 1) - 15) )
                {
                    Doc->cursor.col = i - 1;
                    if ( flag )
                        {
                        chinese_count();
                        english_count();
                        refresh_screen();
                        }
                    break;
                } else if ( x + hbar_height >= word_width * (Doc->cursor.row_ptr->cur_len -Doc->cursor.chi_cnt * 1.08+ 1) - 15 )
                {

                    Doc->cursor.col = Doc->cursor.row_ptr->cur_len;
                    if ( flag )
                        {
                        chinese_count();
                        english_count();
                        refresh_screen();
                        }
                    break;
                }
            }
        }
    }
    //y大于最大行数
    if ( y + vbar_height >= word_height * Doc->cursor.cur_height + toolbar_height )
    {
        while ( Doc->cursor.row_ptr->next_row )
            Doc->cursor.row_ptr = Doc->cursor.row_ptr->next_row;
        Doc->cursor.row_cnt = Doc->cursor.cur_height;
        for ( int i = 0; i <= Doc->cursor.row_ptr->cur_len; i++ )
        {
            int j = i;
            Doc->cursor.chi_cnt = chinese_judge(Doc->cursor.row_ptr,j);
            if ( x + hbar_height > (word_width * (i-Doc->cursor.chi_cnt * 1.08) - 15) && x + hbar_height <= (word_width * (i-Doc->cursor.chi_cnt * 1.08 + 1) - 15) )
            {
                Doc->cursor.col = i - 1;
                if ( flag )
                    {
                    chinese_count();
                    english_count();
                    refresh_screen();
                    }
                break;
            } else if ( x + hbar_height >= word_width * (Doc->cursor.row_ptr->cur_len -Doc->cursor.chi_cnt * 1.08+ 1) - 15 )
            {
                Doc->cursor.col = Doc->cursor.row_ptr->cur_len;
                if ( flag )
                    {
                    chinese_count();
                    english_count();
                    refresh_screen();
                    }
                break;
            }
        }
    }
}

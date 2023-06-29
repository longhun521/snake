#include <QApplication>  // 应用程序类，用于创建和管理应用程序
#include <QWidget>       // 窗口基类，用于创建和管理窗口部件
#include <QLabel>        // 标签类，用于显示文本或图像
#include <QPushButton>   // 按钮类，用于用户交互
#include <QPainter>      // 绘图类，用于在窗口上进行绘图操作
#include <QVector>       // 动态数组类，用于存储和操作一组元素
#include <QKeyEvent>     // 键盘事件类，用于捕捉和处理键盘事件
#include <QTime>         // 时间类，用于获取和操作时间
#include <QFont>         // 字体类，用于设置文本的字体样式
#include <QPixmap>       // 图片类，用于加载和显示图像
#include <QMessageBox>   // 消息框类，用于显示提示和警告信息
#include <QTimer>        // 定时器类，用于创建定时器，定时触发事件
#include <QSoundEffect>  // 音效类，用于播放音效
#include <QDebug>        // 调试输出类，用于输出调试信息
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

const int SIZE = 30;  // 格子大小
const int W = 40;     // 游戏区域宽度（格子数）
const int H = 20;     // 游戏区域高度（格子数）
const int SPEED = 200;  // 蛇移动的速度（毫秒）

enum Direction  // 方向枚举
{
    Up,
    Down,
    Left,
    Right
};

struct Position  // 坐标结构体
{
    int x;
    int y;
};



class LoginWidget : public QWidget
{
private:
    bool isLoggedIn = false;  // 登录状态，默认为false

public:
    LoginWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("登录与注册");

        // 创建用户名标签和文本框
        QLabel* usernameLabel = new QLabel("用户名:", this);
        QLineEdit* usernameLineEdit = new QLineEdit(this);

        // 创建密码标签和文本框
        QLabel* passwordLabel = new QLabel("密码:", this);
        QLineEdit* passwordLineEdit = new QLineEdit(this);
        passwordLineEdit->setEchoMode(QLineEdit::Password); // 设置密码文本框的回显模式为密码

        // 创建登录和注册按钮
        QPushButton* loginButton = new QPushButton("登录", this);
        QPushButton* registerButton = new QPushButton("注册", this);

        // 登录和注册按钮的布局
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(loginButton);
        buttonLayout->addWidget(registerButton);

        // 主布局
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(usernameLabel);
        mainLayout->addWidget(usernameLineEdit);
        mainLayout->addWidget(passwordLabel);
        mainLayout->addWidget(passwordLineEdit);
        mainLayout->addLayout(buttonLayout);

        // 将登录和注册按钮的信号连接到对应的槽函数
        connect(loginButton, &QPushButton::clicked, this, [this, usernameLineEdit, passwordLineEdit]() {
            if (this->login(usernameLineEdit->text(), passwordLineEdit->text())) {
                // 登录成功，隐藏登录窗口
                this->hide();
            }
        });

        connect(registerButton, &QPushButton::clicked, this, [this, usernameLineEdit, passwordLineEdit]() {
            this->registerUser(usernameLineEdit->text(), passwordLineEdit->text());
        });
    }

    bool login(const QString& username, const QString& password)
    {
        if (this->checkCredentials(username, password))
        {
            QMessageBox::information(this, "登录", "登录成功！");
            isLoggedIn = true; // 登录成功后，将登录状态设置为true
            return true;
        }
        else
        {
            QMessageBox::warning(this, "登录", "用户名或密码错误！");
            return false;
        }
    }

    bool getLoginStatus() const { return isLoggedIn; } // 获取当前登录状态

    void registerUser(const QString& username, const QString& password)
    {
        if (username.isEmpty() || password.isEmpty())
        {
            QMessageBox::warning(this, "注册", "用户名和密码不能为空！");
            return;
        }

        if (checkUsernameExists(username))
        {
            QMessageBox::warning(this, "注册", "用户名已存在！");
            return;
        }

        if (saveUser(username, password))
        {
            QMessageBox::information(this, "注册", "注册成功！");
        }
        else
        {
            QMessageBox::critical(this, "注册", "注册失败，请重试！");
        }
    }

    bool checkCredentials(const QString& username, const QString& password)
    {
        // 从文件中加载用户数据
        QFile file("F://snake plus2//user.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "错误", "无法打开用户数据文件！");
            return false;
        }

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2 && parts[0] == username && parts[1] == password)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    bool checkUsernameExists(const QString& username)
    {
        // 从文件中加载用户数据
        QFile file("F://snake plus2//user.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "错误", "无法打开用户数据文件！");
            return false;
        }

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2 && parts[0] == username)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    bool saveUser(const QString& username, const QString& password)
    {
        // 将用户数据保存到文件中
        QFile file("F://snake plus2//user.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::critical(this, "错误", "无法打开用户数据文件！");
            return false;
        }

        QTextStream out(&file);
        out << username << "," << password << "\n";

        file.close();
        return true;
    }
};



class SnakeGame : public QWidget
{

public:
    SnakeGame(QWidget* parent = nullptr) : QWidget(parent)
    {
        // 创建登录窗口
              loginWidget = new LoginWidget(this);
             loginWidget->show();

        setWindowTitle("贪吃蛇大作战");
        setFixedSize(W * SIZE, H * SIZE + 60);  // 设置窗口大小（宽度为游戏区域宽度，高度为游戏区域高度加上额外的60像素）

        setFocusPolicy(Qt::StrongFocus);  // 设置焦点策略，使窗口能够接收键盘事件

        timer = new QTimer(this);  // 创建定时器
        connect(timer, &QTimer::timeout, this, &SnakeGame::move);  // 定时器触发timeout信号时，调用move槽函数

        backgroundImg1.load(":/1.jpg");  // 加载第一张背景图片
        backgroundImg2.load(":/2.jpg");  // 加载第二张背景图片

        score = 0;
        currentLevel = 1;
        isPaused = false;  // 初始化为未暂停状态

        sound = new QSoundEffect(this);  // 创建音效对象
        sound->setSource(QUrl::fromLocalFile(":/tanchishe.wav"));  // 设置音效文件
        sound->setLoopCount(QSoundEffect::Infinite);  // 设置音效循环播放
        sound->setVolume(0.5);  // 设置音效音量
        musicEnabled = true;  // 初始化音乐开关为开启状态
        sound->play();  // 播放音效

        startButton = new QPushButton("", this);  // 创建开始按钮
        startButton->setFixedSize(QSize(1200, 700));  // 设置按钮大小
        startButton->setStyleSheet("QPushButton {border-image: url(:/1.jpg);}");  // 设置按钮样式为背景图片

        connect(startButton, &QPushButton::clicked, this, &SnakeGame::startGame);  // 按下开始按钮时，调用startGame槽函数

        nextButton = new QPushButton("", this);  // 创建下一关按钮
        nextButton->setFixedSize(QSize(180, 140));  // 设置按钮大小
        nextButton->move(width() / 2 - nextButton->width() / 2, height() / 2 - nextButton->height() / 2);  // 设置按钮位置居中
        nextButton->hide();  // 隐藏按钮
        nextButton->setStyleSheet("QPushButton {border-image: url(:/next.jpg);}");  // 设置按钮样式为背景图片
        connect(nextButton, &QPushButton::clicked, this, &SnakeGame::nextLevel);  // 按下下一关按钮时，调用nextLevel槽函数

        scoreLabel = new QLabel(this);  // 创建分数标签
        scoreLabel->setText("   0   ");  // 设置初始分数文本
        scoreLabel->setFont(QFont("Microsoft YaHei", 30));  // 设置文本字体样式
        scoreLabel->setStyleSheet("color: black;");  // 设置文本颜色为黑色
        scoreLabel->move(width() + 10 - scoreLabel->width() - 80, 45);  // 设置文本位置
        scoreLabel->hide();  // 隐藏分数标签（初始状态下不显示）

        levelLabel = new QLabel(this);  // 创建关卡标签
        levelLabel->setText("关卡 1");  // 设置初始关卡文本
        levelLabel->setFont(QFont("Microsoft YaHei", 35));  // 设置文本字体样式
        levelLabel->setStyleSheet("color: black;");  // 设置文本颜色为黑色
        levelLabel->move(width() / 2 - 20 - levelLabel->width() / 2, 45);  // 设置文本位置
        levelLabel->hide();  // 隐藏关卡标签（初始状态下不显示）

        currentImg = backgroundImg1;  // 设置当前背景图片为第一张背景图片


    }

protected:
    void paintEvent(QPaintEvent*) override   //绘画
    {
        QPainter painter(this);  // 创建绘图对象
        painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿渲染

        // 绘制背景图片
        painter.drawPixmap(rect(), currentImg);

        // 绘制食物
        painter.drawPixmap(food.x * SIZE, food.y * SIZE, foodPixmap);

        // 绘制蛇
        for (int i = 0; i < snake.size(); ++i)
        {
            if (i == 0)
                painter.setBrush(snakeHeadColor);  // 头部颜色
            else
                painter.setBrush(snakeBodyColor);  // 身体颜色
            painter.drawRect(snake[i].x * SIZE, snake[i].y * SIZE, SIZE, SIZE);  // 绘制矩形
        }

        // 绘制音乐开关按钮
        QLinearGradient gradient(width() - 100, H * SIZE + 10, width(), H * SIZE + 10);  // 创建线性渐变对象
        gradient.setColorAt(0, musicEnabled ? Qt::green : Qt::red);  // 根据音乐开关状态设置渐变颜色
        gradient.setColorAt(1, Qt::white);  // 设置渐变颜色

        QBrush brush(gradient);  // 创建渐变画刷对象
        painter.setBrush(brush);  // 设置画刷
        painter.drawRoundedRect(QRect(width() - 100, H * SIZE + 10, 80, 30), 20, 20);  // 绘制圆角矩形
        painter.drawText(QRect(width() - 100, H * SIZE + 10, 80, 30), Qt::AlignCenter, musicEnabled ? "音乐开" : "音乐关");  // 绘制文本
    }

    void keyPressEvent(QKeyEvent* event) override  //键盘输入与处理
    {
        switch (event->key())
        {
            case Qt::Key_W:
            case Qt::Key_Up:
                if (direction != Down)
                    direction = Up;
                break;
            case Qt::Key_S:
            case Qt::Key_Down:
                if (direction != Up)
                    direction = Down;
                break;
            case Qt::Key_A:
            case Qt::Key_Left:
                if (direction != Right)
                    direction = Left;
                break;
            case Qt::Key_D:
            case Qt::Key_Right:
                if (direction != Left)
                    direction = Right;
                break;
            case Qt::Key_K:
                toggleMusic();
                break;
            case Qt::Key_Shift:
                acceleration = true;
                speed /= 2;
                break;
            case Qt::Key_Space:
                if (isPaused)
                {
                    timer->start(speed);
                    isPaused = false;
                }
                else
                {
                    timer->stop();
                    isPaused = true;
                }
                break;
            default:
                // 不执行任何操作
                break;
        }
    }

    void keyReleaseEvent(QKeyEvent* event) override  //键盘输入释放
    {
        if (event->key() == Qt::Key_Shift)
        {
            acceleration = false;
            speed *= 2;
        }
    }

private:                                    //成员变量的声明部分
    int totalScore;
    QPixmap foodPixmap;
    bool isPaused;  // 游戏是否暂停的标志
      LoginWidget* loginWidget;  // 登录窗口对象

private slots:                      //声明私有的槽函数。槽函数是用于处理信号的成员函数
    void initializeSnake()
    {
        snake.clear();              // 清空蛇的位置信息
        snake.push_back({ W / 2, H / 2 });   // 在游戏区域中央放置蛇头
        direction = Right;          // 设置蛇的初始移动方向为右侧
        score = 0;                  // 初始化分数为0
        acceleration = false;       // 初始化加速标志为false（表示未加速）
    }

    void startGame()
    {
        loginWidget->hide();  // 隐藏登录界面

        initializeSnake();          // 初始化蛇的位置、方向和分数等信息

        totalScore = 0;             // 初始化总分为0
        currentLevel = 1;           // 初始化当前关卡为1
        speed = SPEED;              // 初始化游戏速度

        generateFood();             // 生成食物

        timer->start(SPEED);        // 启动定时器，控制蛇的移动速度

        scoreLabel->setText("得分:0   ");    // 设置分数标签文本
        levelLabel->setText("第 1 关");       // 设置关卡标签文本
        startButton->hide();        // 隐藏开始按钮
        scoreLabel->show();         // 显示等级和分数标签
        levelLabel->show();

        currentImg = backgroundImg2;    // 设置当前背景图片为第二张背景图片

        update();                   // 更新游戏界面

    }

    void nextLevel()
       {
           initializeSnake();          // 初始化蛇的位置、方向和分数等信息

           currentLevel++;             // 增加当前关卡数
           speed -= 50;                // 加快游戏速度

           generateFood();             // 生成食物

           timer->start(speed);        // 启动定时器，控制蛇的移动速度

           scoreLabel->setText("  0 ");                // 设置分数标签文本
           levelLabel->setText("等级: " + QString::number(currentLevel));   // 设置关卡标签文本
           nextButton->setEnabled(false);              // 禁用下一关按钮
           nextButton->move(width() / 2 - nextButton->width() / 2, height() / 2 - nextButton->height() / 2);   // 移动下一关按钮到界面中央
           nextButton->hide();         // 隐藏下一关按钮

           currentImg = backgroundImg2;    // 设置当前背景图片为第二张背景图片

           update();                   // 更新游戏界面
       }


    void move()
    {
        int dx = 0, dy = 0;         // 初始化水平和垂直移动的增量为0
        switch (direction)          // 根据当前移动方向确定增量值
        {
            case Up:
                dy = -1;            // 向上移动，垂直增量为-1
                break;
            case Down:
                dy = 1;             // 向下移动，垂直增量为1
                break;
            case Left:
                dx = -1;            // 向左移动，水平增量为-1
                break;
            case Right:
                dx = 1;             // 向右移动，水平增量为1
                break;
        }

        int newX = snake[0].x + dx;  // 计算蛇头的新位置坐标
        int newY = snake[0].y + dy;

        if (newX < 0 || newX >= W || newY < 0 || newY >= H + 2 || isSnakeBody(newX, newY))
        {
            gameOver();             // 如果蛇撞到墙壁或自身，则游戏结束
            return;
        }

        snake.push_front({ newX, newY });   // 将新的蛇头位置添加到蛇的位置列表的首部

        if (newX == food.x && newY == food.y)    // 如果蛇头吃到食物
        {
            totalScore += foodScore;    // 增加总分和当前关卡分数
            score += foodScore;
            scoreLabel->setText(" " + QString::number(score));   // 更新分数标签文本
            generateFood();             // 生成新的食物
            if (acceleration)           // 如果加速标志为true
            {
                snake.push_front({ newX, newY });   // 再次将新的蛇头位置添加到蛇的位置列表的首部
            }
        }
        else
        {
            snake.pop_back();           // 如果蛇没有吃到食物，则删除蛇尾，使蛇移动起来
        }

        update();                       // 更新游戏界面

        timer->setInterval(speed);      // 设置定时器的时间间隔为当前速度

        if (score >= levelTarget)       // 如果当前分数达到关卡目标分数
        {
            nextButton->setEnabled(true);   // 启用下一关按钮
            nextButton->show();          // 显示下一关按钮
            timer->stop();               // 停止定时器，暂停游戏
        }
    }

    bool isSnakeBody(int x, int y)
    {
        for (const Position& pos : snake)   // 遍历蛇的每个位置
        {
            if (pos.x == x && pos.y == y)   // 如果某个位置的坐标与给定的坐标相同，则说明蛇的身体包含该位置，返回true
                return true;
        }
        return false;                      // 蛇的身体中不包含给定位置，返回false
    }

    void generateFood()
    {
        qsrand(QTime::currentTime().msec());  // 设置随机数种子

        int x, y;
        do {
            x = qrand() % W;         // 生成随机的食物位置坐标
            y = qrand() % H;
        } while (isSnakeBody(x, y));   // 如果食物位置与蛇的身体重合，则重新生成

        food = { x, y };              // 设置食物的位置

        // 使用自定义图片替代食物
        QImage foodImage(":/xg.jpg");    // 加载食物图片
        foodImage = foodImage.scaled(45, 45);   // 调整图像大小以适应食物
        foodPixmap = QPixmap::fromImage(foodImage);   // 设置食物图片
        foodScore = (qrand() % 3 + 1) * 10;    // 生成随机的食物分数
    }

    void gameOver()
    {
        timer->stop();                  // 停止定时器

        // 创建音效对象
         QSoundEffect sound1;
         sound1.setSource(QUrl::fromLocalFile(":/over.wav"));
         sound1.setVolume(1.0);

         sound->stop();
         sound1.play();



        QMessageBox::information(this, "贪吃蛇大作战", QString("游戏结束\n你的总分是: %1").arg(totalScore));   // 显示游戏结束信息框，包含总分

        QMessageBox::StandardButton restartButton = QMessageBox::question(this, "贪吃蛇大作战",
            QString("你还想重新挑战一次吗?"), QMessageBox::Yes | QMessageBox::No);
        if (restartButton == QMessageBox::Yes)
        {
            QMessageBox::information(this, "贪吃蛇大作战", "好的，游戏开始");
            startGame();
        }
        else
        {
            QMessageBox::information(this, "贪吃蛇大作战", "欢迎下次再来");
            QApplication::quit();
        }
    }

    void toggleMusic()
    {
        musicEnabled = !musicEnabled;   // 切换音乐开关

        if (musicEnabled)
        {
            sound->play();
        }
        else
        {
            sound->stop();
        }
    }

private:              //成员函数和槽函数的声明部分
    QTimer* timer;  // 定时器
    QVector<Position> snake;  // 蛇的身体位置
    Position food;  // 食物的位置
    QColor foodColor;  // 食物颜色
    QColor snakeHeadColor = QColor(0, 255, 0);  // 蛇头颜色
    QColor snakeBodyColor = QColor(0, 200, 0);  // 蛇身颜色
    Direction direction = Right;  // 蛇的移动方向
    int score;  // 当前分数
    int currentLevel;  // 当前关卡
    int speed;  // 蛇移动的速度
    bool acceleration;  // 是否加速
    int levelTarget = 100;  // 每关卡达到的目标分数
    QPixmap backgroundImg1;  // 第一张背景图片
    QPixmap backgroundImg2;  // 第二张背景图片
    QPixmap currentImg;  // 当前背景图片
    QLabel* scoreLabel;  // 分数标签
    QLabel* levelLabel;  // 关卡标签
    QPushButton* startButton;  // 开始按钮
    QPushButton* nextButton;  // 下一关按钮
    int foodScore;  // 食物分数
    QSoundEffect* sound;  // 音效对象
    bool musicEnabled;  // 音乐开关状态
};



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 创建登录窗口
    LoginWidget loginWidget;
    loginWidget.show();

    // 使用事件循环等待用户操作
    do {
        app.processEvents();
        // 检查用户是否已经登录
        if (loginWidget.getLoginStatus())
        {
            try {
                QMessageBox::information(nullptr, "贪吃蛇大作战", "欢迎来到贪吃蛇大作战！\n 1.按WASD或上下左右方向键可以实现蛇移动方向的改变。\n 2.按住Shift可加速，松开恢复原速。\n 3.按下空格键可暂停，再次按下可继续。\n 4.游戏界面展示当前关卡和分数，每关卡达到100分可进入\n 下一关。游戏结束后会显示总分数。");
                SnakeGame game;
                game.show();
                return app.exec();
            }
            catch (std::exception& e) {
                qDebug() << "Exception caught: " << e.what();
                return 1;
            }
        }
    } while (loginWidget.isVisible());

    return 0;
}

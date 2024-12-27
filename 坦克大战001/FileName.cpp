#include <graphics.h>		// EasyX引用图形库头文件
#include <conio.h>
#include<iostream>
using namespace std;

#define WIDTH 630		// 窗口宽度
#define HEIGHT 630		// 窗口高度
#define W 0		// 上移
#define S 1		// 下移
#define A 2		// 左移
#define D 3		// 右移
#define SPEED 6	// 坦克移动速度
#define BULLET 10	// 子弹移动速度
#define MAX_BULLETS 50		// 假设最多有50个子弹
#define ENEMY 6 // 敌人坦克数量
#define SIZE 3	// 边框像素
#define PX 24	// 像素转换比
#define ARR 26 // 地图方块数

// 坦克类
class Tank {
private:
	int x, y;	// 位置
	int distance;	// 方向
	int width, height;
	bool alive;	// 是否存活
	int flag;
public:
	void setDistance(int dis) {
		this->distance = dis;
	}
	int getDistance() {
		return distance;
	}
	void setX(int x) {
		this->x = x;
	}
	int getX() {
		return x;
	}
	void setY(int y) {
		this->y = y;
	}
	int getY() {
		return y;
	}
	void setWidth(int width) {
		this->width = width;
	}
	int getWidth() {
		return width;
	}
	void setHeight(int height) {
		this->height = height;
	}
	int getHeight() {
		return height;
	}
	void setAlive(bool alive) {
		this->alive = alive;
	}
	bool getAlive() {
		return alive;
	}
	void setFlag(int flag) {
		this->flag = flag;
	}
	int getFlag() {
		return flag;
	}
};

class MyTank :public Tank {

};

class AiTank :public Tank {

};

// 子弹类
class Bullet :public Tank {

};

//void initAiOnly(Tank* tank, int i);		// 控制敌人生成位置坐标不重叠
void creatBullet(Tank& tank, Bullet* bullets, int flag);		// 创建子弹
bool isInWall(Bullet& bullets);			// 子弹与墙碰撞检测
void isInEnemy(Bullet& bullet, Tank& tank);		// 子弹碰到坦克处理
void pause();		// 暂停游戏
bool gameover(Tank& myTank, Tank* aiTank);			//游戏结束

// 设置地图为全局变量
int map[ARR][ARR] = {
	//map里的值: 0为可通过陆地，1为砖，5为我方坦克，9为我方基地，3代表敌人可能出现的位置
	{ 2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,3,2,2,2,2,0},
	{ 2,0,0,0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,0,0,2,0,0,0,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 3,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,3,0},
	{ 2,0,1,1,3,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,0,0,1,1,2,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,0,0,2,0,0,0,0,0,1,1,3,0,1,1,2,0},
	{ 2,0,1,1,2,0,1,1,0,0,0,0,2,0,0,0,0,0,1,1,2,0,1,1,2,0},
	{ 2,2,2,2,2,2,2,2,2,0,1,1,2,0,1,1,2,2,2,2,2,2,2,2,2,0},
	{ 2,2,2,0,0,0,0,0,2,0,1,1,2,0,1,1,2,0,0,0,0,0,2,2,2,0},
	{ 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{ 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0},
	{ 0,0,0,0,0,0,0,0,0,5,0,1,9,9,1,0,0,0,0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,0,0,0,1,9,9,1,0,0,0,0,0,0,0,0,0,0,0}
};

// 设置背景
void setBackground(IMAGE& background) {
	loadimage(&background, _T("./坦克大战素材包/background.png"));
	putimage(0, 0, &background);		// 设置新的背景
}


// 初始化墙体
void initWall(IMAGE& wall) {
	loadimage(&wall, _T("./坦克大战素材包/brick.png"));
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1) {
				putimage(j * PX + SIZE, i * PX + SIZE, &wall);
			}
		}
	}
}

// 初始化我方基地
void initHome(IMAGE& home) {
	loadimage(&home, _T("./坦克大战素材包/home.png"));
	putimage(12 * PX + SIZE, 24 * PX + SIZE, &home);		// // 我方基地位于图中map[24][12]
}

// 初始化我方坦克
void initMytank(Tank& tank, IMAGE& tank_1) {;
	loadimage(&tank_1, _T("./坦克大战素材包/tank_1.png"));
	tank.setAlive(1);
	tank.setFlag(1);
	tank.setWidth(tank_1.getwidth() / 2);
	tank.setHeight(tank_1.getheight() / 4);
	tank.setX(9 * PX + SIZE);		// 我方坦克位于图中map[24][9]
	tank.setY(24 * PX + SIZE);
	// x,y坐标							绘制图像的宽度/高度			当前图像左上角（0,0）为原点开始绘制
	putimage(tank.getX(), tank.getY(), tank.getWidth(), tank.getHeight(), &tank_1, 0, tank.getHeight() * tank.getDistance());
}

// 初始化敌方坦克
void initAiTank(Tank* tank, IMAGE& enemyAI) {
	loadimage(&enemyAI, _T("./坦克大战素材包/enemy_1_1.png"));

	int i = 0;
	for (int y = 0; y < ARR; y++) {
		for (int x = 0; x < ARR; x++) {
			if (map[y][x] == 3) {
				tank[i].setAlive(1);
				tank[i].setFlag(0);
				tank[i].setWidth(enemyAI.getwidth() / 2);
				tank[i].setHeight(enemyAI.getheight() / 4);
				tank[i].setX(x * PX + SIZE);
				tank[i].setY(y * PX + SIZE);
				putimage(tank[i].getX(), tank[i].getY(), tank[i].getWidth(), tank[i].getHeight(), &enemyAI, 0, tank[i].getHeight() * tank[i].getDistance());
				i++;
			}
		}
	}
}

// 绘制背景
void drawBackground(IMAGE& background) {
	putimage(0, 0, &background);		// 重新绘制背景
}

// 绘制墙体
void drawWall(IMAGE& wall) {
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1) {
				putimage(j * PX + SIZE, i * PX + SIZE, &wall);
			}
		}
	}
}

// 绘制我方基地
void drawHome(IMAGE& home) {
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 9) {
				putimage(j * PX + SIZE, i * PX + SIZE, &home);
				return;		// 只绘制一次 
			}
		}
	}
	//putimage(12 * PX + SIZE, 24 * PX + SIZE, &home);
}

// 绘制坦克
void drawTank(Tank& tank, IMAGE& tank_1) {
	// x,y坐标	绘制图像的宽度/高度		当前图像左上角（0,0）为原点开始绘制
	if (tank.getAlive()) {
		putimage(tank.getX(), tank.getY(), tank.getWidth(), tank.getHeight(), &tank_1, 0, tank.getHeight() * tank.getDistance());
	}
}
// 绘制坦克重载，绘制敌方多辆坦克
void drawTank(Tank* tank, IMAGE& tank_1) {
	for (int i = 0; i < ENEMY; i++) {
		drawTank(tank[i], tank_1);
	}
}

// 检测两个坦克是否碰撞,true碰撞
bool isCollisions(Tank& tank1, Tank& tank2) {
	return !(tank1.getX() + tank1.getWidth() <= tank2.getX() ||		// 矩形1在矩形2左侧
		tank1.getX() >= tank2.getX() + tank2.getWidth() ||			// 矩形1在矩形2右侧
		tank1.getY() + tank1.getHeight() <= tank2.getY() ||			// 矩形1在矩形2上方
		tank1.getY() >= tank2.getY() + tank2.getHeight());			// 矩形1在矩形2下方
}

// 我方坦克与敌方坦克碰撞检测
void checkMyTank(Tank& myTank, Tank* aiTank) {
	for (int i = 0; i < ENEMY; i++) {
		// 如果我方坦克碰到敌方坦克	，我方和敌方坦克都阵亡
		if (aiTank[i].getAlive() && isCollisions(myTank,aiTank[i])) {
			//myTank.setAlive(0);
			aiTank[i].setAlive(0);
			myTank.setAlive(0);
		}
	}
}

// 敌方坦克与敌方坦克碰撞检测
void checkMyTank(Tank& myTank, Tank* aiTank, int num) {
	for (int i = 0; i < ENEMY; i++) {
		if (aiTank[i].getAlive() && isCollisions(myTank, aiTank[i]) && i != num) {
			// 回到原来的位置
			switch (myTank.getDistance()) {
			case 0: // 向上
				myTank.setY(myTank.getY() + SPEED);
				break;
			case 1: // 向下
				myTank.setY(myTank.getY() - SPEED);
				break;
			case 2: // 向左
				myTank.setX(myTank.getX() + SPEED);
				break;
			case 3: // 向右
				myTank.setX(myTank.getX() - SPEED);
				break;
			}
		}
	}
}

// 检测坦克是否碰撞到墙体	或   基地
bool isInWall(Tank& nowTank) {
	// 墙体碰撞检测
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1 || map[i][j] == 9) {
				if (!(nowTank.getX() + nowTank.getWidth()-1<= j * PX + SIZE ||	// 碰到墙返回true
					nowTank.getX() +1 >= (j + 1) * PX + SIZE ||
					nowTank.getY() + nowTank.getHeight()-1 <= i * PX + SIZE ||
					nowTank.getY()+1 >= (i + 1) * PX + SIZE)) {
					// 回到原来的位置
					switch (nowTank.getDistance()) {
					case 0: // 向上
						nowTank.setY(nowTank.getY() + SPEED);
						break;
					case 1: // 向下
						nowTank.setY(nowTank.getY() - SPEED);
						break;
					case 2: // 向左
						nowTank.setX(nowTank.getX() + SPEED);
						break;
					case 3: // 向右
						nowTank.setX(nowTank.getX() - SPEED);
						break;
					}
					return true;
				}
			}
		}
	}
	return false;
}

// 我方碰撞检测
void checkCollisions(Tank& nowTank, Tank* otherTank, Bullet* bullets) {
	// 边界检查，防止坦克移出屏幕
	if (nowTank.getX() < SIZE) nowTank.setX(SIZE);
	if (nowTank.getX() > WIDTH - nowTank.getWidth() - SIZE) nowTank.setX(WIDTH - nowTank.getWidth() - SIZE);
	if (nowTank.getY() < SIZE) nowTank.setY(SIZE);
	if (nowTank.getY() > HEIGHT - nowTank.getHeight() - SIZE) nowTank.setY(HEIGHT - nowTank.getHeight() - SIZE);

	// 坦克与墙碰撞检测
	isInWall(nowTank);
	// 坦克碰到坦克检测
	checkMyTank(nowTank, otherTank);
	// 坦克碰到子弹检测
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].getAlive()) {
			isInEnemy(bullets[i], nowTank);
		}
	}
}

// 敌方碰撞检测重载
void checkCollisions(Tank& nowTank, Tank* otherTank,int num,Bullet* bullets) {
	// 边界检查，防止坦克移出屏幕，坦克随机转向
	if (nowTank.getX() < SIZE) {
		nowTank.setX(SIZE);
		nowTank.setDistance(rand() % 4);  
	}
	if (nowTank.getX() > WIDTH - nowTank.getWidth() - SIZE) {
		nowTank.setX(WIDTH - nowTank.getWidth() - SIZE);
		nowTank.setDistance(rand() % 4); 
	}
	if (nowTank.getY() < SIZE) {
		nowTank.setY(SIZE);
		nowTank.setDistance(rand() % 4); 
	}
	if (nowTank.getY() > HEIGHT - nowTank.getHeight() - SIZE) {
		nowTank.setY(HEIGHT - nowTank.getHeight() - SIZE);
		nowTank.setDistance(rand() % 4);  
	}
	if (isInWall(nowTank)) {
		nowTank.setDistance((nowTank.getDistance() + 2) % 4);  // 左转/右转
	}
	
	// 坦克碰到坦克检测
	checkMyTank(nowTank, otherTank, num);

	// 坦克碰到子弹检测
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (otherTank[i].getAlive()) {
			isInEnemy(bullets[i], nowTank);
		}
	}
}

// 我方移动坦克
void moveTank(Tank& tank, IMAGE& tank_1, Bullet* bullets, Tank* aiTank, IMAGE& home) {
	if (!tank.getAlive()) return;  // 如果坦克已死亡，不移动
	if (_kbhit()) {		// 使程序不因键盘的输入而停止，实现类似多线程效果
		int key = _getch();	// 获取键盘信息的,可以通过比较字符'A'或者虚拟键值比较
		if (!gameover(tank, aiTank)) {		// 游戏进行时，我方坦克才可以移动
			switch (key) {
			case 'w':		// w上移
			case 'W':
				tank.setY(tank.getY() - SPEED);
				tank.setDistance(W);
				break;
			case 's':		// s下移
			case 'S':
				tank.setY(tank.getY() + SPEED);
				tank.setDistance(S);
				break;
			case 'a':		// a左移
			case 'A':
				tank.setX(tank.getX() - SPEED);
				tank.setDistance(A);
				break;
			case 'd':		// d右移
			case 'D':
				tank.setX(tank.getX() + SPEED);
				tank.setDistance(D);
				break;
			case 'F':		// f发射子弹
			case 'f':
				creatBullet(tank, bullets, 1);	// 我方子弹为1
				break;
			case ' ':
				pause();
				break;
			case 27:  // ESC 键退出循环
				exit(1);
			}
		}
		else {
			if (key == 27) {
				exit(1);
			}
			//else if (key == ' ') {
			//	//drawHome(home);			// 初始化我方基地
			//	map[24][12] = 9;
			//	return;
			//}
		}
	}
}

// 敌方坦克移动
void moveAiTank(Tank* tank, IMAGE& enemy_1, Bullet* bullets, Tank& myTank) {
	// 游戏进行时，敌方坦克才可以移动
	if (!gameover(myTank, tank)) {
		for (int i = 0; i < ENEMY; i++) {
			if (!tank[i].getAlive()) continue;  // 如果坦克已死亡，死亡的坦克不移动

			if (rand() % 10 == 0) {
				tank[i].setDistance(rand() % 4);
			}

			if (rand() % 20 == 0) {
				creatBullet(tank[i], bullets, 0);	// 敌方子弹为0
			}

			switch (tank[i].getDistance()) {
			case W:		// w上移
				tank[i].setY(tank[i].getY() - SPEED);
				break;
			case S:		// s下移
				tank[i].setY(tank[i].getY() + SPEED);
				break;
			case A:		// a左移
				tank[i].setX(tank[i].getX() - SPEED);
				break;
			case D:		// d右移
				tank[i].setX(tank[i].getX() + SPEED);
				break;
			}

			// 敌方碰撞检测
			checkCollisions(tank[i], tank, i, bullets);
		}
	}
}

 // 初始化子弹
void initBulletT(IMAGE& bulletT, IMAGE& bulletD, IMAGE& bulletL, IMAGE& bulletR, Bullet* bullets) {
	loadimage(&bulletT, _T("./坦克大战素材包/bullet_up.png"));
	loadimage(&bulletD, _T("./坦克大战素材包/bullet_down.png"));
	loadimage(&bulletL, _T("./坦克大战素材包/bullet_left.png"));
	loadimage(&bulletR, _T("./坦克大战素材包/bullet_right.png"));
	for (int i = 0; i < MAX_BULLETS;i++) {
		bullets[i].setAlive(0);
		bullets[i].setFlag(0);
		bullets[i].setWidth(bulletT.getwidth());
		bullets[i].setHeight(bulletT.getheight());
	}
}

// 创建子弹
void creatBullet(Tank& tank, Bullet* bullets,int flag) {	// flag判断是敌方创建的子弹还是我方,0敌方，1我方
	// 查找第一个未使用的子弹
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (!bullets[i].getAlive()) {
			// 激活子弹
			bullets[i].setAlive(true);

			// 根据坦克的方向设置子弹的初始位置
			switch (tank.getDistance()) {
			case W: // 向上
				bullets[i].setX(tank.getX() + tank.getWidth() / 2 - bullets[i].getWidth() / 2);
				bullets[i].setY(tank.getY());
				bullets[i].setDistance(W);
				bullets[i].setFlag(flag);
				break;
			case S: // 向下
				bullets[i].setX(tank.getX() + tank.getWidth() / 2 - bullets[i].getWidth() / 2);
				bullets[i].setY(tank.getY() + tank.getHeight());
				bullets[i].setDistance(S);
				bullets[i].setFlag(flag);
				break;
			case A: // 向左
				bullets[i].setX(tank.getX());
				bullets[i].setY(tank.getY() + tank.getHeight() / 2 - bullets[i].getHeight() / 2);
				bullets[i].setDistance(A);
				bullets[i].setFlag(flag);
				break;
			case D: // 向右
				bullets[i].setX(tank.getX() + tank.getWidth());
				bullets[i].setY(tank.getY() + tank.getHeight() / 2 - bullets[i].getHeight() / 2);
				bullets[i].setDistance(D);
				bullets[i].setFlag(flag);
				break;
			}
			return;
		}
	}
}

// 绘制子弹
void drawBullet(IMAGE& bulletT, IMAGE& bulletD, IMAGE& bulletL, IMAGE& bulletR, Bullet* bullets, Tank& myTank,Tank* aiTank) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (!gameover(myTank, aiTank)) {
			if (bullets[i].getAlive()) {
				switch (bullets[i].getDistance()) {
				case W:		// w上移
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletT);
					bullets[i].setY(bullets[i].getY() - BULLET);
					break;
				case S:		// s下移
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletD);
					bullets[i].setY(bullets[i].getY() + BULLET);
					break;
				case A:		// a左移
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletL);
					bullets[i].setX(bullets[i].getX() - BULLET);
					break;
				case D:		// d右移
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletR);
					bullets[i].setX(bullets[i].getX() + BULLET);
					break;
				}
			}
		}
		// 检查子弹是否超出屏幕边界  或  碰到墙体
		if (bullets[i].getX() < SIZE || bullets[i].getX() + bullets[i].getWidth() > WIDTH - SIZE ||
			bullets[i].getY() < SIZE || bullets[i].getY() + bullets[i].getHeight() > HEIGHT - SIZE ||
			isInWall(bullets[i])) {
			bullets[i].setFlag(0);		// 设置子弹为初始状态
			bullets[i].setAlive(false); // 子弹超出屏幕，标记为死亡
		}
	}
}

// 检测子弹是否碰撞到墙体	或	我方基地
bool isInWall(Bullet& bullets) {
	// 墙体碰撞检测
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1 || map[i][j] == 9) {
				if (!(bullets.getX() + bullets.getWidth() <= j * PX + SIZE ||	// 碰到墙返回true
					bullets.getX() >= (j + 1) * PX + SIZE ||
					bullets.getY() + bullets.getHeight() <= i * PX + SIZE ||
					bullets.getY() >= (i + 1) * PX + SIZE)) {
					if (map[i][j] == 9) {	// 打到的是基地,基地消失
						map[i][j] = 0;
						map[i + 1][j] = 0;
						map[i][j + 1] = 0;
						map[i + 1][j + 1] = 0;
					}
					else {
						map[i][j] = 0;		// 子弹碰到墙，墙消失
					}
					return true;
				}
			}
		}
	}
	return false;
}

// 检测子弹是否碰到敌人，碰到返回true
bool isCollisions(Bullet& bullet, Tank& tank) {
	return !(bullet.getX() + bullet.getWidth() <= tank.getX() ||		// 子弹在坦克左侧
		bullet.getX() >= tank.getX() + tank.getWidth() ||				// 子弹在坦克右侧
		bullet.getY() + bullet.getHeight() <= tank.getY() ||			// 子弹在坦克上方
		bullet.getY() >= tank.getY() + tank.getHeight() ||				// 子弹在坦克下方
		bullet.getFlag() == tank.getFlag());							// 子弹和坦克编号一致等于没碰撞
}

// 子弹碰到坦克处理
void isInEnemy(Bullet& bullet, Tank& tank) {
	if (isCollisions(bullet, tank)) {
		tank.setAlive(false);
		bullet.setAlive(false);
	}
}

// 初始化游戏结束画面
void initOver(IMAGE& over) {
	loadimage(&over, _T("./坦克大战素材包/gameover.png"));
}

 //游戏结束
bool gameover(Tank& myTank, Tank* aiTank) {
	int num = 0;
	for (int i = 0; i < ENEMY; i++) {
		if (!aiTank[i].getAlive()) {
			num++;
		}
	}
	if (num == ENEMY) return true;		// 敌人坦克全部阵亡
	if (!myTank.getAlive()) return true;	// 我方坦克阵亡
	if (map[24][12] == 0) return true;	// 我方基地被摧毁
		
	return false;
}

// 绘制游戏结束画面
void drawOver(Tank& myTank, Tank* aiTank,IMAGE& over) {
	if (gameover(myTank,aiTank)) {
		putimage(WIDTH / 2 - over.getwidth() / 2, HEIGHT / 2 - over.getheight() / 2, &over);
	}
}

// 暂停游戏
void pause() {
	while (1) {
		int key = _getch();	// 等待输入
		if (key == ' ') {	// 按空格继续游戏
			break;
		}
		else if (key == 27) {
			exit(1);		// esc退出游戏
		}
	}
}

int main()
{
	srand(unsigned int(time(NULL)));
	MyTank myTank;
	myTank.setDistance(W);
	AiTank aiTank[ENEMY];		// 6个敌人数组，随机方向
	for (int i = 0; i < ENEMY; i++) {
		aiTank[i].setDistance(rand()%4);
	}
	Bullet bullets[50];		// 假设最多50个子弹

	initgraph(WIDTH, HEIGHT);	// 创建绘图窗口，大小为 630×630 像素

	setbkcolor(BLUE);		// 设置背景色
	cleardevice();			// 将绘图区填充为当前背景色
	IMAGE background;	
	setBackground(background);	// 设置背景

	IMAGE wall;
	initWall(wall);			// 初始化墙体

	IMAGE home;
	initHome(home);				// 初始化我方基地

	IMAGE tank_1;
	initMytank(myTank, tank_1);	// 初始化我方坦克

	IMAGE enemy_1;			
	initAiTank(aiTank, enemy_1);	// 初始化敌方坦克

	IMAGE over;			// 游戏结束画面
	initOver(over);

	IMAGE bulletT;		// 上子弹
	IMAGE bulletD;		// 下子弹
	IMAGE bulletL;		// 左子弹
	IMAGE bulletR;		// 右子弹
	initBulletT(bulletT, bulletD, bulletL, bulletR, bullets);
	
	while (1) {
		moveTank(myTank, tank_1, bullets,aiTank, home);	//移动我方坦克
		// 我方碰撞检测
		checkCollisions(myTank, aiTank, bullets);
		moveAiTank(aiTank, enemy_1,bullets,myTank);	// 敌方坦克自动移动
		
		//双缓冲绘图，需要放在绘图代码之前和之后，为了解决画面闪烁问题
		BeginBatchDraw();//开始批量绘图

		// 清除旧的图像
		cleardevice();
		drawBackground(background);		// 绘制背景
		drawWall(wall);	// 绘制墙体
		drawHome(home);	// 绘制我方基地
		drawTank(myTank, tank_1);	// 绘制我方坦克
		drawTank(aiTank, enemy_1);	// 绘制敌方坦克
		drawBullet(bulletT, bulletD, bulletL, bulletR, bullets, myTank, aiTank);		// 绘制子弹
		drawOver(myTank, aiTank, over);	// 游戏结束

		FlushBatchDraw();//清空批量绘图

		Sleep(50);	// 控制帧率
	}
	
	EndBatchDraw();		//  结束批量绘制
	getchar();			// 等待用户关闭窗口
	closegraph();			// 关闭绘图窗口
	return 0;
}
#include <graphics.h>		// EasyX����ͼ�ο�ͷ�ļ�
#include <conio.h>
#include<iostream>
using namespace std;

#define WIDTH 630		// ���ڿ��
#define HEIGHT 630		// ���ڸ߶�
#define W 0		// ����
#define S 1		// ����
#define A 2		// ����
#define D 3		// ����
#define SPEED 6	// ̹���ƶ��ٶ�
#define BULLET 10	// �ӵ��ƶ��ٶ�
#define MAX_BULLETS 50		// ���������50���ӵ�
#define ENEMY 6 // ����̹������
#define SIZE 3	// �߿�����
#define PX 24	// ����ת����
#define ARR 26 // ��ͼ������

// ̹����
class Tank {
private:
	int x, y;	// λ��
	int distance;	// ����
	int width, height;
	bool alive;	// �Ƿ���
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

// �ӵ���
class Bullet :public Tank {

};

//void initAiOnly(Tank* tank, int i);		// ���Ƶ�������λ�����겻�ص�
void creatBullet(Tank& tank, Bullet* bullets, int flag);		// �����ӵ�
bool isInWall(Bullet& bullets);			// �ӵ���ǽ��ײ���
void isInEnemy(Bullet& bullet, Tank& tank);		// �ӵ�����̹�˴���
void pause();		// ��ͣ��Ϸ
bool gameover(Tank& myTank, Tank* aiTank);			//��Ϸ����

// ���õ�ͼΪȫ�ֱ���
int map[ARR][ARR] = {
	//map���ֵ: 0Ϊ��ͨ��½�أ�1Ϊש��5Ϊ�ҷ�̹�ˣ�9Ϊ�ҷ����أ�3������˿��ܳ��ֵ�λ��
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

// ���ñ���
void setBackground(IMAGE& background) {
	loadimage(&background, _T("./̹�˴�ս�زİ�/background.png"));
	putimage(0, 0, &background);		// �����µı���
}


// ��ʼ��ǽ��
void initWall(IMAGE& wall) {
	loadimage(&wall, _T("./̹�˴�ս�زİ�/brick.png"));
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1) {
				putimage(j * PX + SIZE, i * PX + SIZE, &wall);
			}
		}
	}
}

// ��ʼ���ҷ�����
void initHome(IMAGE& home) {
	loadimage(&home, _T("./̹�˴�ս�زİ�/home.png"));
	putimage(12 * PX + SIZE, 24 * PX + SIZE, &home);		// // �ҷ�����λ��ͼ��map[24][12]
}

// ��ʼ���ҷ�̹��
void initMytank(Tank& tank, IMAGE& tank_1) {;
	loadimage(&tank_1, _T("./̹�˴�ս�زİ�/tank_1.png"));
	tank.setAlive(1);
	tank.setFlag(1);
	tank.setWidth(tank_1.getwidth() / 2);
	tank.setHeight(tank_1.getheight() / 4);
	tank.setX(9 * PX + SIZE);		// �ҷ�̹��λ��ͼ��map[24][9]
	tank.setY(24 * PX + SIZE);
	// x,y����							����ͼ��Ŀ��/�߶�			��ǰͼ�����Ͻǣ�0,0��Ϊԭ�㿪ʼ����
	putimage(tank.getX(), tank.getY(), tank.getWidth(), tank.getHeight(), &tank_1, 0, tank.getHeight() * tank.getDistance());
}

// ��ʼ���з�̹��
void initAiTank(Tank* tank, IMAGE& enemyAI) {
	loadimage(&enemyAI, _T("./̹�˴�ս�زİ�/enemy_1_1.png"));

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

// ���Ʊ���
void drawBackground(IMAGE& background) {
	putimage(0, 0, &background);		// ���»��Ʊ���
}

// ����ǽ��
void drawWall(IMAGE& wall) {
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1) {
				putimage(j * PX + SIZE, i * PX + SIZE, &wall);
			}
		}
	}
}

// �����ҷ�����
void drawHome(IMAGE& home) {
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 9) {
				putimage(j * PX + SIZE, i * PX + SIZE, &home);
				return;		// ֻ����һ�� 
			}
		}
	}
	//putimage(12 * PX + SIZE, 24 * PX + SIZE, &home);
}

// ����̹��
void drawTank(Tank& tank, IMAGE& tank_1) {
	// x,y����	����ͼ��Ŀ��/�߶�		��ǰͼ�����Ͻǣ�0,0��Ϊԭ�㿪ʼ����
	if (tank.getAlive()) {
		putimage(tank.getX(), tank.getY(), tank.getWidth(), tank.getHeight(), &tank_1, 0, tank.getHeight() * tank.getDistance());
	}
}
// ����̹�����أ����Ƶз�����̹��
void drawTank(Tank* tank, IMAGE& tank_1) {
	for (int i = 0; i < ENEMY; i++) {
		drawTank(tank[i], tank_1);
	}
}

// �������̹���Ƿ���ײ,true��ײ
bool isCollisions(Tank& tank1, Tank& tank2) {
	return !(tank1.getX() + tank1.getWidth() <= tank2.getX() ||		// ����1�ھ���2���
		tank1.getX() >= tank2.getX() + tank2.getWidth() ||			// ����1�ھ���2�Ҳ�
		tank1.getY() + tank1.getHeight() <= tank2.getY() ||			// ����1�ھ���2�Ϸ�
		tank1.getY() >= tank2.getY() + tank2.getHeight());			// ����1�ھ���2�·�
}

// �ҷ�̹����з�̹����ײ���
void checkMyTank(Tank& myTank, Tank* aiTank) {
	for (int i = 0; i < ENEMY; i++) {
		// ����ҷ�̹�������з�̹��	���ҷ��͵з�̹�˶�����
		if (aiTank[i].getAlive() && isCollisions(myTank,aiTank[i])) {
			//myTank.setAlive(0);
			aiTank[i].setAlive(0);
			myTank.setAlive(0);
		}
	}
}

// �з�̹����з�̹����ײ���
void checkMyTank(Tank& myTank, Tank* aiTank, int num) {
	for (int i = 0; i < ENEMY; i++) {
		if (aiTank[i].getAlive() && isCollisions(myTank, aiTank[i]) && i != num) {
			// �ص�ԭ����λ��
			switch (myTank.getDistance()) {
			case 0: // ����
				myTank.setY(myTank.getY() + SPEED);
				break;
			case 1: // ����
				myTank.setY(myTank.getY() - SPEED);
				break;
			case 2: // ����
				myTank.setX(myTank.getX() + SPEED);
				break;
			case 3: // ����
				myTank.setX(myTank.getX() - SPEED);
				break;
			}
		}
	}
}

// ���̹���Ƿ���ײ��ǽ��	��   ����
bool isInWall(Tank& nowTank) {
	// ǽ����ײ���
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1 || map[i][j] == 9) {
				if (!(nowTank.getX() + nowTank.getWidth()-1<= j * PX + SIZE ||	// ����ǽ����true
					nowTank.getX() +1 >= (j + 1) * PX + SIZE ||
					nowTank.getY() + nowTank.getHeight()-1 <= i * PX + SIZE ||
					nowTank.getY()+1 >= (i + 1) * PX + SIZE)) {
					// �ص�ԭ����λ��
					switch (nowTank.getDistance()) {
					case 0: // ����
						nowTank.setY(nowTank.getY() + SPEED);
						break;
					case 1: // ����
						nowTank.setY(nowTank.getY() - SPEED);
						break;
					case 2: // ����
						nowTank.setX(nowTank.getX() + SPEED);
						break;
					case 3: // ����
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

// �ҷ���ײ���
void checkCollisions(Tank& nowTank, Tank* otherTank, Bullet* bullets) {
	// �߽��飬��ֹ̹���Ƴ���Ļ
	if (nowTank.getX() < SIZE) nowTank.setX(SIZE);
	if (nowTank.getX() > WIDTH - nowTank.getWidth() - SIZE) nowTank.setX(WIDTH - nowTank.getWidth() - SIZE);
	if (nowTank.getY() < SIZE) nowTank.setY(SIZE);
	if (nowTank.getY() > HEIGHT - nowTank.getHeight() - SIZE) nowTank.setY(HEIGHT - nowTank.getHeight() - SIZE);

	// ̹����ǽ��ײ���
	isInWall(nowTank);
	// ̹������̹�˼��
	checkMyTank(nowTank, otherTank);
	// ̹�������ӵ����
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].getAlive()) {
			isInEnemy(bullets[i], nowTank);
		}
	}
}

// �з���ײ�������
void checkCollisions(Tank& nowTank, Tank* otherTank,int num,Bullet* bullets) {
	// �߽��飬��ֹ̹���Ƴ���Ļ��̹�����ת��
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
		nowTank.setDistance((nowTank.getDistance() + 2) % 4);  // ��ת/��ת
	}
	
	// ̹������̹�˼��
	checkMyTank(nowTank, otherTank, num);

	// ̹�������ӵ����
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (otherTank[i].getAlive()) {
			isInEnemy(bullets[i], nowTank);
		}
	}
}

// �ҷ��ƶ�̹��
void moveTank(Tank& tank, IMAGE& tank_1, Bullet* bullets, Tank* aiTank, IMAGE& home) {
	if (!tank.getAlive()) return;  // ���̹�������������ƶ�
	if (_kbhit()) {		// ʹ��������̵������ֹͣ��ʵ�����ƶ��߳�Ч��
		int key = _getch();	// ��ȡ������Ϣ��,����ͨ���Ƚ��ַ�'A'���������ֵ�Ƚ�
		if (!gameover(tank, aiTank)) {		// ��Ϸ����ʱ���ҷ�̹�˲ſ����ƶ�
			switch (key) {
			case 'w':		// w����
			case 'W':
				tank.setY(tank.getY() - SPEED);
				tank.setDistance(W);
				break;
			case 's':		// s����
			case 'S':
				tank.setY(tank.getY() + SPEED);
				tank.setDistance(S);
				break;
			case 'a':		// a����
			case 'A':
				tank.setX(tank.getX() - SPEED);
				tank.setDistance(A);
				break;
			case 'd':		// d����
			case 'D':
				tank.setX(tank.getX() + SPEED);
				tank.setDistance(D);
				break;
			case 'F':		// f�����ӵ�
			case 'f':
				creatBullet(tank, bullets, 1);	// �ҷ��ӵ�Ϊ1
				break;
			case ' ':
				pause();
				break;
			case 27:  // ESC ���˳�ѭ��
				exit(1);
			}
		}
		else {
			if (key == 27) {
				exit(1);
			}
			//else if (key == ' ') {
			//	//drawHome(home);			// ��ʼ���ҷ�����
			//	map[24][12] = 9;
			//	return;
			//}
		}
	}
}

// �з�̹���ƶ�
void moveAiTank(Tank* tank, IMAGE& enemy_1, Bullet* bullets, Tank& myTank) {
	// ��Ϸ����ʱ���з�̹�˲ſ����ƶ�
	if (!gameover(myTank, tank)) {
		for (int i = 0; i < ENEMY; i++) {
			if (!tank[i].getAlive()) continue;  // ���̹����������������̹�˲��ƶ�

			if (rand() % 10 == 0) {
				tank[i].setDistance(rand() % 4);
			}

			if (rand() % 20 == 0) {
				creatBullet(tank[i], bullets, 0);	// �з��ӵ�Ϊ0
			}

			switch (tank[i].getDistance()) {
			case W:		// w����
				tank[i].setY(tank[i].getY() - SPEED);
				break;
			case S:		// s����
				tank[i].setY(tank[i].getY() + SPEED);
				break;
			case A:		// a����
				tank[i].setX(tank[i].getX() - SPEED);
				break;
			case D:		// d����
				tank[i].setX(tank[i].getX() + SPEED);
				break;
			}

			// �з���ײ���
			checkCollisions(tank[i], tank, i, bullets);
		}
	}
}

 // ��ʼ���ӵ�
void initBulletT(IMAGE& bulletT, IMAGE& bulletD, IMAGE& bulletL, IMAGE& bulletR, Bullet* bullets) {
	loadimage(&bulletT, _T("./̹�˴�ս�زİ�/bullet_up.png"));
	loadimage(&bulletD, _T("./̹�˴�ս�زİ�/bullet_down.png"));
	loadimage(&bulletL, _T("./̹�˴�ս�زİ�/bullet_left.png"));
	loadimage(&bulletR, _T("./̹�˴�ս�زİ�/bullet_right.png"));
	for (int i = 0; i < MAX_BULLETS;i++) {
		bullets[i].setAlive(0);
		bullets[i].setFlag(0);
		bullets[i].setWidth(bulletT.getwidth());
		bullets[i].setHeight(bulletT.getheight());
	}
}

// �����ӵ�
void creatBullet(Tank& tank, Bullet* bullets,int flag) {	// flag�ж��ǵз��������ӵ������ҷ�,0�з���1�ҷ�
	// ���ҵ�һ��δʹ�õ��ӵ�
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (!bullets[i].getAlive()) {
			// �����ӵ�
			bullets[i].setAlive(true);

			// ����̹�˵ķ��������ӵ��ĳ�ʼλ��
			switch (tank.getDistance()) {
			case W: // ����
				bullets[i].setX(tank.getX() + tank.getWidth() / 2 - bullets[i].getWidth() / 2);
				bullets[i].setY(tank.getY());
				bullets[i].setDistance(W);
				bullets[i].setFlag(flag);
				break;
			case S: // ����
				bullets[i].setX(tank.getX() + tank.getWidth() / 2 - bullets[i].getWidth() / 2);
				bullets[i].setY(tank.getY() + tank.getHeight());
				bullets[i].setDistance(S);
				bullets[i].setFlag(flag);
				break;
			case A: // ����
				bullets[i].setX(tank.getX());
				bullets[i].setY(tank.getY() + tank.getHeight() / 2 - bullets[i].getHeight() / 2);
				bullets[i].setDistance(A);
				bullets[i].setFlag(flag);
				break;
			case D: // ����
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

// �����ӵ�
void drawBullet(IMAGE& bulletT, IMAGE& bulletD, IMAGE& bulletL, IMAGE& bulletR, Bullet* bullets, Tank& myTank,Tank* aiTank) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (!gameover(myTank, aiTank)) {
			if (bullets[i].getAlive()) {
				switch (bullets[i].getDistance()) {
				case W:		// w����
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletT);
					bullets[i].setY(bullets[i].getY() - BULLET);
					break;
				case S:		// s����
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletD);
					bullets[i].setY(bullets[i].getY() + BULLET);
					break;
				case A:		// a����
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletL);
					bullets[i].setX(bullets[i].getX() - BULLET);
					break;
				case D:		// d����
					putimage(bullets[i].getX(), bullets[i].getY(), &bulletR);
					bullets[i].setX(bullets[i].getX() + BULLET);
					break;
				}
			}
		}
		// ����ӵ��Ƿ񳬳���Ļ�߽�  ��  ����ǽ��
		if (bullets[i].getX() < SIZE || bullets[i].getX() + bullets[i].getWidth() > WIDTH - SIZE ||
			bullets[i].getY() < SIZE || bullets[i].getY() + bullets[i].getHeight() > HEIGHT - SIZE ||
			isInWall(bullets[i])) {
			bullets[i].setFlag(0);		// �����ӵ�Ϊ��ʼ״̬
			bullets[i].setAlive(false); // �ӵ�������Ļ�����Ϊ����
		}
	}
}

// ����ӵ��Ƿ���ײ��ǽ��	��	�ҷ�����
bool isInWall(Bullet& bullets) {
	// ǽ����ײ���
	for (int i = 0; i < ARR; i++) {
		for (int j = 0; j < ARR; j++) {
			if (map[i][j] == 1 || map[i][j] == 9) {
				if (!(bullets.getX() + bullets.getWidth() <= j * PX + SIZE ||	// ����ǽ����true
					bullets.getX() >= (j + 1) * PX + SIZE ||
					bullets.getY() + bullets.getHeight() <= i * PX + SIZE ||
					bullets.getY() >= (i + 1) * PX + SIZE)) {
					if (map[i][j] == 9) {	// �򵽵��ǻ���,������ʧ
						map[i][j] = 0;
						map[i + 1][j] = 0;
						map[i][j + 1] = 0;
						map[i + 1][j + 1] = 0;
					}
					else {
						map[i][j] = 0;		// �ӵ�����ǽ��ǽ��ʧ
					}
					return true;
				}
			}
		}
	}
	return false;
}

// ����ӵ��Ƿ��������ˣ���������true
bool isCollisions(Bullet& bullet, Tank& tank) {
	return !(bullet.getX() + bullet.getWidth() <= tank.getX() ||		// �ӵ���̹�����
		bullet.getX() >= tank.getX() + tank.getWidth() ||				// �ӵ���̹���Ҳ�
		bullet.getY() + bullet.getHeight() <= tank.getY() ||			// �ӵ���̹���Ϸ�
		bullet.getY() >= tank.getY() + tank.getHeight() ||				// �ӵ���̹���·�
		bullet.getFlag() == tank.getFlag());							// �ӵ���̹�˱��һ�µ���û��ײ
}

// �ӵ�����̹�˴���
void isInEnemy(Bullet& bullet, Tank& tank) {
	if (isCollisions(bullet, tank)) {
		tank.setAlive(false);
		bullet.setAlive(false);
	}
}

// ��ʼ����Ϸ��������
void initOver(IMAGE& over) {
	loadimage(&over, _T("./̹�˴�ս�زİ�/gameover.png"));
}

 //��Ϸ����
bool gameover(Tank& myTank, Tank* aiTank) {
	int num = 0;
	for (int i = 0; i < ENEMY; i++) {
		if (!aiTank[i].getAlive()) {
			num++;
		}
	}
	if (num == ENEMY) return true;		// ����̹��ȫ������
	if (!myTank.getAlive()) return true;	// �ҷ�̹������
	if (map[24][12] == 0) return true;	// �ҷ����ر��ݻ�
		
	return false;
}

// ������Ϸ��������
void drawOver(Tank& myTank, Tank* aiTank,IMAGE& over) {
	if (gameover(myTank,aiTank)) {
		putimage(WIDTH / 2 - over.getwidth() / 2, HEIGHT / 2 - over.getheight() / 2, &over);
	}
}

// ��ͣ��Ϸ
void pause() {
	while (1) {
		int key = _getch();	// �ȴ�����
		if (key == ' ') {	// ���ո������Ϸ
			break;
		}
		else if (key == 27) {
			exit(1);		// esc�˳���Ϸ
		}
	}
}

int main()
{
	srand(unsigned int(time(NULL)));
	MyTank myTank;
	myTank.setDistance(W);
	AiTank aiTank[ENEMY];		// 6���������飬�������
	for (int i = 0; i < ENEMY; i++) {
		aiTank[i].setDistance(rand()%4);
	}
	Bullet bullets[50];		// �������50���ӵ�

	initgraph(WIDTH, HEIGHT);	// ������ͼ���ڣ���СΪ 630��630 ����

	setbkcolor(BLUE);		// ���ñ���ɫ
	cleardevice();			// ����ͼ�����Ϊ��ǰ����ɫ
	IMAGE background;	
	setBackground(background);	// ���ñ���

	IMAGE wall;
	initWall(wall);			// ��ʼ��ǽ��

	IMAGE home;
	initHome(home);				// ��ʼ���ҷ�����

	IMAGE tank_1;
	initMytank(myTank, tank_1);	// ��ʼ���ҷ�̹��

	IMAGE enemy_1;			
	initAiTank(aiTank, enemy_1);	// ��ʼ���з�̹��

	IMAGE over;			// ��Ϸ��������
	initOver(over);

	IMAGE bulletT;		// ���ӵ�
	IMAGE bulletD;		// ���ӵ�
	IMAGE bulletL;		// ���ӵ�
	IMAGE bulletR;		// ���ӵ�
	initBulletT(bulletT, bulletD, bulletL, bulletR, bullets);
	
	while (1) {
		moveTank(myTank, tank_1, bullets,aiTank, home);	//�ƶ��ҷ�̹��
		// �ҷ���ײ���
		checkCollisions(myTank, aiTank, bullets);
		moveAiTank(aiTank, enemy_1,bullets,myTank);	// �з�̹���Զ��ƶ�
		
		//˫�����ͼ����Ҫ���ڻ�ͼ����֮ǰ��֮��Ϊ�˽��������˸����
		BeginBatchDraw();//��ʼ������ͼ

		// ����ɵ�ͼ��
		cleardevice();
		drawBackground(background);		// ���Ʊ���
		drawWall(wall);	// ����ǽ��
		drawHome(home);	// �����ҷ�����
		drawTank(myTank, tank_1);	// �����ҷ�̹��
		drawTank(aiTank, enemy_1);	// ���Ƶз�̹��
		drawBullet(bulletT, bulletD, bulletL, bulletR, bullets, myTank, aiTank);		// �����ӵ�
		drawOver(myTank, aiTank, over);	// ��Ϸ����

		FlushBatchDraw();//���������ͼ

		Sleep(50);	// ����֡��
	}
	
	EndBatchDraw();		//  ������������
	getchar();			// �ȴ��û��رմ���
	closegraph();			// �رջ�ͼ����
	return 0;
}
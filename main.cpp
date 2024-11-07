#include<graphics.h> //easyxͼ��ͷ�ļ�
#include<time.h>
#include<conio.h>//����̨
#include<math.h>
IMAGE imgBg;

struct Point {
	int x, y;
};
const int ROW_COUNT = 20;//20��
const int COL_COUNT = 10;//10��
int table[ROW_COUNT][COL_COUNT] = { 0 };//�հ�

struct Point curBlock[4];//curBlock[2]��ʾ�����½��Ķ���˹�����ڲ��ĵڶ���С�����λ��
struct Point BackBlock[4];//���ݣ���ʾ�½������е���һ��λ��

int blockIndex;//��ʾ�½��Ķ���˹���������


const int normal_speed = 500;//�����ʼ���ٶ�
const int high_speed = 50;//����
int delay = normal_speed;
bool update = false;

int blocks[7][4] = {
	1,3,5,7,
	2,4,5,7,
	3,5,4,7,
	3,5,4,6,
	3,5,7,6,
	2,3,5,7,
	2,3,4,5,
};

IMAGE blocksImg[7];

void newBlock() {
	//rand()����һ���������0���������
	blockIndex = 1 + rand() % 7;
	int i;
	for (i = 0; i < 4; i++) {
		int M = blocks[blockIndex - 1][i];
		curBlock[i].y = M / 2;
		curBlock[i].x = M % 2;//��ǰС��������λ��
	}


}  
bool judge() {     //�ж��Ƿ�Խ��
	for (int i = 0; i < 4; i++) {
		if (curBlock[i].x < 0 || curBlock[i].x >= 10 || curBlock[i].y < 0 || curBlock[i].y >= 20 || table[curBlock[i].y]
			[curBlock[i].x] != 0) {
			return false;
		}
	}return true;
	 }


void drop() {
	for (int i = 0; i < 4; i++) {
		BackBlock[i] = curBlock[i];
		curBlock[i].y += 1;//�½�һ��
	}
	if (judge() == false) {
		for (int i = 0; i < 4; i++) {
			table[BackBlock[i].y][BackBlock[i].x] = blockIndex;
		}newBlock();//��ʱ����һ���µķ���
	}
}

void init() { //��ʼ��׼��������������Ϸ����
	initgraph(392,800);
	// ���ر���ͼƬ
	loadimage(&imgBg, _T("picture/backgrouond2.jpg"));
	srand(time(NULL));

	IMAGE blocks;//����7��Сͼ��Ĵ�ͼ
	loadimage(&blocks, _T("picture/block.2.jpg"),317.6,39.7,true);
	for (int i = 1; i <= 7; i++) {
		SetWorkingImage(&blocks);
		getimage(&blocksImg[i - 1],39.7*i,0,39.7,39.7);
	}
	SetWorkingImage();//�ָ�ͼƬ������



}

void draw() { //�����ײ��ķ���
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (table[i][j] == 0) { continue; }  
			int x = j * 39.7 ;
			int y = i * 39.7 ;
			int m = table[i][j];
			putimage(x, y, &blocksImg[m - 1]);
		}
	}
	for (int i = 0; i < 4; i++) { //δ���䵽�ײ��ķ���		
		int x = curBlock[i].x * 39.7;
			int y = curBlock[i].y * 39.7;
		int m = blockIndex - 1;
		putimage(x, y, &blocksImg[m]);
	}
}

int getDelay() { //������ʱ
	static unsigned long long lasttime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lasttime == 0) {
		lasttime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lasttime;
		lasttime = currentTime;
		return ret;
	}
}

void moveLeftRight(int offeset) {//�����ƶ�
	for (int i = 0; i < 4; i++) {
		BackBlock[i] = curBlock[i];
		curBlock[i].x += offeset;
	}
	if (!judge()) {
		for (int i = 0; i < 4; i++) {
			curBlock[i] = BackBlock[i];
		}
	}
}
void doRotate(void) { //������ת

	for (int i = 0; i < 4; i++) {
		BackBlock[i] = curBlock[i];
	}
	Point p = curBlock[1];//���ĵķ���
	for (int i = 0; i < 4; i++) {
		struct Point tmp = curBlock[i];
		curBlock[i].x = p.x + p.y - tmp.y;
		curBlock[i].y = p.y - p.x + tmp.x;
	}
	if (!judge()) {
		for (int i = 0; i < 4; i++) {
			curBlock[i] = BackBlock[i];
		}
	}
}



void keyEvent() { //�������ҷ����
	int k = 0;
	bool rotate = false;
	unsigned char ch = 0;
	while (_kbhit()) {
		unsigned char ch = _getch();
		if (ch == 224) {
			ch = _getch();

			if (ch == 72) { rotate = true;  }
			else if (ch == 80) { delay = high_speed;  }
			else if (ch == 75) {
				k = -1;
				
			}
			else if (ch == 77) {
				k = 1;
				
			}
		}

	}

	if (k != 0) {
		moveLeftRight(k);
		update = true;
	}

	if(rotate) { //up��������ת
		doRotate();
		update = true;
	}
}


	void elimate(){ //ʵ������
		int m = ROW_COUNT - 1;
		for (int i = ROW_COUNT - 1; i > 0; i--) {
			int count = 0;
			for (int j = 0; j < COL_COUNT; j++) {
				if (table[i][j]) count++;
				table[m][j] = table[i][j];
			}
			if (count < COL_COUNT) m--;
			else {
				update = true;
			}
		}

	}
	void drawscore() {
		setcolor(BLACK);
		settextstyle(60, 0, _T("����"));
			char scoreText[32];
		setbkmode(TRANSPARENT);
	//	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
		outtextxy(255 * 2, 175 * 2, scoreText);
	}
	


	



int main(void) {
	init();

	newBlock();

	float timer = 0;
	while (1) {
		int time=getDelay();
		timer += time;
		keyEvent();//����ʱ��
		if (timer > delay) {
			drop(); timer = 0;
			update = true;
		}
		elimate();
		
		delay = normal_speed;//�ٶȻ�ԭ
		if (update) {
			putimage(0, 0, &imgBg);//���»�����
			draw();
			drawscore();
			update = false;
		}
		
	}
	


	system("pause");//��ͣ
	closegraph();//�ر�ͼ�δ���
	return 0;
}

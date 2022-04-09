#include <iostream>
#include <glut.h>
#include <math.h>
#include <windows.h>
using namespace std;

const double PI = 3.14159265;

/**曲柄滑块配置**/
GLfloat L1 = 2.5f;      //短杆长度
GLfloat L2 = 8.0f;      //长杆长度
GLfloat h = 3.0f;  // 载体悬空的高度，比正方体（滑块）高

GLfloat alpha = 0.0f;     // 短杆的转角
GLfloat beta = 0.0f;    // 长杆的转角
GLfloat theta = 0.0f;	//长杆与水平线的夹角

GLfloat xRot = 0.0f;			//绕X轴的转角
GLfloat yRot = 0.0f;			//绕Y轴的转角


GLfloat pos_x = 0.0f;  // 短杆端点 
GLfloat pos_y = 0.0f;

GLfloat H = 0.0f; // 短杆端点到正方体所在平面的垂直距离

GLfloat X = 0.0f;	//正方体载体的的水平距离
GLfloat L = 0.0f;		//正方体到载体的距离 斜着的边
GLfloat Z = 0.0f;	//长杆的投影长度



/**轴角度配置**/
GLfloat axisX_angle = 0.0f;
GLfloat axisY_angle = 0.0f;
GLfloat axisZ_angle = 0.0f;

/**观察配置**/
GLfloat eye_x = 0.0f;
GLfloat eye_y = 0.0f;
GLfloat eye_z = 6.0f;


/**光照配置**/
static GLfloat light_position[] = { -10.0, 15.0, 5.0, 1.0 }; //指定光源的位置
static GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 0.0 };    //指定环境光 
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };    //指定漫射光
/**材质配置**/
static GLfloat mat_shininess[] = { 100.0 };  //镜面指数
static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //镜面光


/**鼠标**/
GLenum OP = GLUT_UP; // 指令
int prevx = 0;
int prevy = 0;

/**连续运动标识**/
GLboolean space_flag = false;
GLboolean return_flag = false;

void specialKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void init(void);
void display(void);
void drawScene(void);
void drawFrame(void);
void reshape(int width, int height);
void drawCylinder(GLfloat bottom_radius, GLfloat top_radis, GLfloat height);
void drawBox(GLfloat hwg);
void drawPole(GLfloat radius, GLfloat length);
void drawShaft(GLfloat radius, GLfloat length);
void drawBearing(GLfloat radius, GLfloat height);
void drawCarrier(GLfloat radius, GLfloat height);
void idle(void);
// 鼠标
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON: //鼠标左键
		if (state == GLUT_DOWN) { //鼠标按下
			alpha += 0.1; // 转一个位置
			if (alpha >= 2 * PI)
			{
				alpha = 0.0;
			}
		}
		
		break;
	}
	glutPostRedisplay();
}

// 拉视角
void motion(int x, int y) {
	if (OP == GLUT_DOWN) {
		int deltax, deltay;
		deltax = prevx - x;
		deltay = prevy - y;
		axisX_angle += 0.5 * deltax;
		axisY_angle += 0.5 * deltay;
		if (deltax != 0 && deltay != 0)
			axisZ_angle += 0.5 * sqrt(deltax * deltax + deltay * deltay);

		if (axisX_angle < 0)
			axisX_angle += 360.0;
		if (axisY_angle < 0)
			axisY_angle += 360.0;
		if (axisZ_angle < 0)
			axisZ_angle += 360.0;

		if (axisX_angle > 360.0)
			axisX_angle -= 360.0;
		if (axisY_angle > 360.0)
			axisY_angle -= 360.0;
		if (axisZ_angle > 360.0)
			axisZ_angle -= 360.0;
	}
	else {
		OP = GLUT_UP;
	}
	
	glutPostRedisplay();
}

// 正方体
void drawBox(GLfloat hwg) {
	glBegin(GL_QUADS);
	//绘制前面
	glNormal3f(0.0f, 0.0f, -1.0f);					//面的法向量
	glVertex3f(-hwg / 2.0, -hwg / 2.0, 0.0f);		//顶点
	glVertex3f(-hwg / 2.0, hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, -hwg / 2.0, 0.0f);

	//绘制右面
	glNormal3f(1.0f, 0.0f, 0.0f);							
	glVertex3f(hwg / 2.0, -hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(hwg / 2.0, -hwg / 2.0, hwg);

	//绘制下面
	glNormal3f(0.0f, 1.0f, 0.0f);							
	glVertex3f(-hwg / 2.0, hwg / 2.0, 0.0f);
	glVertex3f(-hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(hwg / 2.0, hwg / 2.0, 0.0f);

	//绘制左面
	glNormal3f(-1.0f, 0.0f, 0.0f);							
	glVertex3f(-hwg / 2.0, -hwg / 2.0, hwg);
	glVertex3f(-hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(-hwg / 2.0, hwg / 2.0, 0.0f);
	glVertex3f(-hwg / 2.0, -hwg / 2.0, 0.0f);

	//绘制上面
	glNormal3f(0.0f, -1.0f, 0.0f);							
	glVertex3f(-hwg / 2.0, -hwg / 2.0, hwg);
	glVertex3f(-hwg / 2.0, -hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, -hwg / 2.0, 0.0f);
	glVertex3f(hwg / 2.0, -hwg / 2.0, hwg);

	//绘制后面
	glNormal3f(0.0f, 0.0f, 1.0f);							
	glVertex3f(hwg / 2.0, -hwg / 2.0, hwg);
	glVertex3f(hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(-hwg / 2.0, hwg / 2.0, hwg);
	glVertex3f(-hwg / 2.0, -hwg / 2.0, hwg);

	glEnd();

}

// 画底座
void drawBottom(GLfloat bottom_radius, GLfloat top_radis, GLfloat height) {

	GLUquadricObj* cylinder;	
	cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	glPushMatrix();	

	gluCylinder(cylinder, bottom_radius, top_radis, height, 50, 50);	//圆柱
	gluQuadricOrientation(cylinder, GLU_INSIDE);
	gluDisk(cylinder, 0.0, bottom_radius, 50, 50);				//封底面
	glTranslatef(0.0f, 0.0f, height);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluDisk(cylinder, 0.0, top_radis, 50, 50);					//封顶面

	glPopMatrix();	

}

// 载体，轴承所安装的位置 一个圆柱
void drawCarrier(GLfloat radius, GLfloat height) {
	GLUquadricObj* cylinder;	
	cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	
	glPushMatrix();	

	gluCylinder(cylinder, radius, radius, height, 50, 50);	// 载体

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, height);				
	glTranslatef(0.0f, 0.0f, 0.2f);					
	gluDisk(cylinder, 0.0f, 0.3, 50, 50);				
	glPopMatrix();

	gluQuadricOrientation(cylinder, GLU_INSIDE);   //封面
	gluDisk(cylinder, 0.0, radius, 50, 50);					

	glPopMatrix();	

}

// 画轴承  一个圆柱
void drawBearing(GLfloat radius, GLfloat height) {
	GLUquadricObj* cylinder;	// Quadric Object
	cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	glPushMatrix();	

	gluCylinder(cylinder, radius, radius, height, 50, 50);	//轴承圆柱
	gluQuadricOrientation(cylinder, GLU_INSIDE);
	gluDisk(cylinder, 0.0, radius, 50, 50);					//封前面
	glTranslatef(0.0f, 0.0f, height);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluDisk(cylinder, 0.0, radius, 50, 50);					//封后面	

	glPopMatrix();	

}

// 画 轴， 轴承上面伸出去的部分
void drawShaft(GLfloat radius, GLfloat length) {
	GLUquadricObj* cylinder;	
	cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);


	glPushMatrix();	

	gluCylinder(cylinder, radius, radius, length, 50, 50);	
	gluQuadricOrientation(cylinder, GLU_INSIDE);
	gluDisk(cylinder, 0.0, radius, 50, 50);					//封前
	glTranslatef(0.0f, 0.0f, length);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluDisk(cylinder, 0.0, radius, 50, 50);					//封后	

	glPopMatrix();	

}

// 画连接杆 （长度）
void drawPole(GLfloat radius, GLfloat length) {
	GLUquadricObj* cylinder;	
	cylinder = gluNewQuadric();
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	glPushMatrix();	
	gluCylinder(cylinder, radius, radius, length, 50, 50);	//连接杆	
	glPopMatrix();	

}



//初始化窗口内的场景
void init(void) {

	//启用阴影平滑
	glShadeModel(GL_SMOOTH);

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 逆时针方向为面的正面
	glFrontFace(GL_CCW);

	// 不计算面的反面内部
	glEnable(GL_CULL_FACE);

	// 设置背景颜色
	glClearColor(0.25f, 0.25f, 0.50f, 1.0f);


	// 开启光源
	glEnable(GL_LIGHTING);

	//定义光源参数
	GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };		//环境光
	GLfloat  diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };		//漫反射光
	GLfloat  specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };		//镜面反射光
	GLfloat	 lightPos[] = { -10.0f, 15.0f, 5.0f, 1.0f };			//光源位置		

	// 设置光源0的参数
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//启用光源0
	glEnable(GL_LIGHT0);
}

void drawFrame()
{

	pos_x = L1 * cos(alpha);	// 	端点			
	pos_y = L1 * sin(alpha);					
	H = h - 1.0 + pos_y;	// 高度			
	X = sqrt((L2 * L2 - H * H));
	theta = asin(H / L2);
	beta = 180 - (theta * 180) / PI;
	L = X + pos_x; // 水平长度

	//glColor3f(1.0f, 1.0f, 1.0f);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清楚缓存
	
	glPushMatrix();

	
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(axisX_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(axisY_angle, 0.0f, 1.0f, 0.0f);
	glRotatef(axisZ_angle, 0.0f, 0.0f, 1.0f);
	
	glScalef(1.0f, 1.0f, 1.0f); // 视图缩放
	drawScene();
	glPushMatrix();

	
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(-3.0f, 0.0f, -4.0f);
	
	//drawBottom(1.0, 0.3, h); // 圆柱底座

	
	glTranslatef(0.0f, 0.0f, h);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.9f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawCarrier(0.5f, 1.7f); // 轴和轴承的载体

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.5f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawShaft(0.1f, 0.5f); //  轴
	glPopMatrix();

	glTranslatef(0.0f, 0.0f, -0.4f);   
	drawBearing(0.3f, 0.3f);   //  轴承

	glTranslatef(0.0f, 0.0f, -0.25f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.4f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(180 * alpha / PI, 1.0f, 0.0f, 0.0f); // 短杆的旋转运动
	
	drawPole(0.14f, L1);   //  短杆

	glTranslatef(0.0f, 0.0f, L1);	
	glPushMatrix();		
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.15f);
	drawBearing(0.3f, 0.3f);  //短杆的另一端  中间的那块，连接短杆和长杆

	
	glTranslatef(0.0f, 0.0f, -0.4f);	
	drawBearing(0.3f, 0.3f);          // 轴承

	
	glTranslatef(0.0f, 0.0f, -0.1f);
	drawShaft(0.1f, 0.9f);  //  轴

	glPopMatrix();

	glPopMatrix();

	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, (h - 1.0), 0.0f);		
	glTranslatef(0.0f, 0.0f, L);		


	glPushMatrix();
	
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.15f);
	drawBearing(0.3f, 0.3f);     // 轴承


	glTranslatef(0.0f, 0.0f, -0.1f);
	drawShaft(0.1f, 0.5f); //  轴

	glTranslatef(0.0f, 0.0f, 0.5f);
	drawBox(2.0f);  // 正方体 = 滑块
	glPopMatrix();

	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(beta, 1.0f, 0.0f, 0.0f);  // 长杆的旋转角度
	drawPole(0.14, L2);   //  长杆

	glPopMatrix();
	glPopMatrix();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h); // 视角
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // 观察
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//画场景
void drawScene(void) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES); //画线
	GLfloat i = 0.0f;
	for (i = -100.0f; i < 100.0f; i += 1.0f)
	{
		glVertex3f(-50.0f, -5.0f, i);
		glVertex3f(100.0f, -5.0f, i);
		glVertex3f(i, -5.0f, -50.0f);
		glVertex3f(i, -5.0f, 100.0f);
	}
	glEnd();
}


//绘制图元
void display(void) {

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//调用OpenGL绘图函数进行图形绘制
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除颜色缓存和深度缓存

	//glDisable(GL_DEPTH_TEST);
	glLoadIdentity();

	glPushMatrix();
	glScalef(80, 70, 1);
	glTranslatef(0, 0, 0.1);

	

	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	drawFrame();//调用虚拟函数，具体绘制OpenGL图形
	glutSwapBuffers(); //缓冲区

}

// 操作曲柄滑块  
// 鼠标左键点一次转一个位置，键盘 上 点一次 是顺时针转一个位置，下 是逆时针 点一次  转一个位置
// 按空格键顺时针连续运动，按回车键逆时针连续运动。按S键停止运动
// https://www.cnblogs.com/penny1141/archive/2012/04/25/2470109.html
void keyBoard(unsigned char key, int x, int y) {

	switch (key) {
		
		case 'S':  //停止
			alpha = 0.0; 
			space_flag = false;
			return_flag = false;
			break;

		case 'R': // 重新开始
			axisX_angle = 0.0f;
			axisY_angle = 0.0f;
			axisZ_angle = 0.0f;
			eye_x = 0.0f;
			eye_y = 0.0f;
			eye_z = 6.0f;
			break;
		default:
			break;
	}


	glutPostRedisplay();
}

void onTimer(GLint event)
{
	if (event == 0 && space_flag == true)
	{
		alpha += 0.01;
		if (alpha >= 2 * PI)
		{
			alpha = 0.0;
		}
	}

	if (event == 0 && return_flag == true)
	{
		alpha -= 0.01;
		if (alpha <= 0.0)
		{
			alpha = 2 * PI;
		}
	}
	
}

// 控制 https://www.jianshu.com/p/985e23cf03c6
void specialKey(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP: // 上
			alpha += 0.1; // 转一个位置
			if (alpha >= 2 * PI)
			{
				alpha = 0.0;
			}
			break;
		case GLUT_KEY_DOWN: // 下
			alpha -= 0.1; // 转一个位置
			if (alpha <= 0.0)
			{
				alpha = 2 * PI;
			}
			break;
		case  GLUT_KEY_LEFT: // 回车
			cout << key << endl;
			return_flag = true;
			space_flag = false;
			break;

		case GLUT_KEY_RIGHT:  // 空格
			cout << key << endl;
			space_flag = true;
			return_flag = false;
			break;

	}
	//if (key == GLUT_KEY_LEFT) {
		
		
	//}

	//if (key == GLUT_KEY_RIGHT) {
		
	//}

	glutPostRedisplay();
}


void idle(void)
{
	glutTimerFunc(50, onTimer, 0.5);
	glutPostRedisplay(); // 刷新

}

//主函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // 初始化
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //设置显示模式
	glutInitWindowPosition(100, 100);//设置窗口在屏幕中的位置
	glutInitWindowSize(1400, 700);//设置窗口的尺寸
	glutCreateWindow("2021262767  孙义: Left代替回车键，Right代替空格键");//设置窗口标题

	init();                                 //初始化场景

	glutDisplayFunc(display);    //显示
	glutReshapeFunc(reshape);
	glutIdleFunc(idle); // 全局回调函数
	glutSpecialFunc(specialKey); 
	glutKeyboardFunc(keyBoard); 
	glutMouseFunc(mouse);

	//glutMotionFunc(motion);

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutMainLoop(); // 主循环
	return 0;
}
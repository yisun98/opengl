#include <iostream>
#include <glut.h>
#include <math.h>
using namespace std;

/** 菜单变量 **/
int speedMenu = 0;
int fillMenu = 0;
int mainMenu = 0;
bool flag = false;

/**光照配置**/
static GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 }; //指定光源的位置
static GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 0.0 };    //指定环境光 
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };    //指定漫射光
/**材质配置**/
static GLfloat mat_shininess[] = { 100.0 };  //镜面指数
static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //镜面光材质

const double PI = 3.14159265;

/**轴角度配置**/
GLfloat axisX_angle = 0.0f;
GLfloat axisY_angle = 0.0f;
GLfloat axisZ_angle = 0.0f;

/**观察配置**/
GLfloat eye_x = 0.0f;
GLfloat eye_y = 0.0f;
GLfloat eye_z = 8.0f;

/**自行车配置**/
GLfloat pedal_angle = 0.0f;
GLfloat run_speed = 0.0f; 
GLfloat position_x = 0.0f; // 位置
GLfloat position_z = 0.0f;
GLfloat toward = 0.0f; // 自行车的转向
GLfloat direction = 0.0f;  //位置方向

/**鼠标**/
GLenum OP = GLUT_UP; // 指令
int prevx = 0;
int prevy = 0;

/**菜单选项**/
int menu_id = 0;


void init(void);
void display(void);
void drawScene(void);
void drawBicycleFrame(void);
void drawCylinder(GLfloat radius, GLfloat length, GLint direction);
void drawGearWheel(GLfloat inner_radius, GLfloat outer_radius, GLfloat height, GLfloat width, GLint number);
void drawChain(void);
void drawPedals(void);
void drawWheel(void);
void drawSeat(void);
void idle(void);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void specialKey(int key, int x, int y);
void keyBoard(unsigned char key, int x, int y);
void menu_item(void);


void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON: //鼠标左键
			if (state == GLUT_DOWN) { //鼠标按下
				OP = GLUT_DOWN;
				prevx = x;
				prevy = y;
			}
			if (state == GLUT_UP) {
				OP = GLUT_UP;
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
	prevx = x;
	prevy = y;
	glutPostRedisplay();
}

// 画座椅
void drawSeat(void) {


	glBegin(GL_POLYGON);
	glVertex3f(-0.1f, -1.0f, -0.5f); // 下面
	glVertex3f(1.0f, -1.0f, -0.3f);
	glVertex3f(1.0f, -1.0f, 0.3f);
	glVertex3f(-0.1f, -1.0f, 0.5f);
	glVertex3f(-0.5f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-0.5f, -1.0f, -1.0f);
	glEnd();

	//中间围起来
	

	//glEnd();


}


// 画轮子，前后轮
void drawWheel(void) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidTorus(0.06f, 0.92f, 4, 30); // 轮毂

	glColor3f(1.0f, 1.0f, 0.5f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.06f);

	drawCylinder(0.02f, 0.12f, 1);
	glPopMatrix();
	glutSolidTorus(0.02f, 0.02f, 3, 20);

	glColor3f(1.0f, 0.0f, 1.0f);
	GLint i = 0;
	// 添加轮子中的线
	for (i = 0; i < 20; ++i)
	{
		glPushMatrix();
		glRotatef(i * 18, 0.0f, 0.0f, 1.0f); //  均匀划分
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.02f, 0.0f);
		glVertex3f(0.0f, 0.86f, 0.0f);
		glEnd();
		glPopMatrix();
	}

	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidTorus(0.08f, 1.0f, 10, 30); // 黑色轮胎
	glColor3f(1.0f, 0.0f, 0.0f);
}



//画脚踏板
void drawPedals(void) {
	glColor3f(1.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.105f);
	glRotatef(-pedal_angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.25f, 0.0f, 0.0f); 

	glPushMatrix();
	glScalef(0.5f, 0.1f, 0.1f);
	glutSolidCube(1.0f);  // 正面的
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.105f);
	glRotatef(180.0f - pedal_angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.25f, 0.0f, 0.0f);

	glPushMatrix();
	glScalef(0.5f, 0.1f, 0.1f);
	glutSolidCube(1.0f); // 反面的
	glPopMatrix();

	glPopMatrix();

	glColor3f(1.0f, 0.0f, 0.0f);

}

//画链子
void drawChain(void) {

	glColor3f(0.0f, 1.0f, 0.0f);
	glEnable(GL_LINE_STIPPLE);  //点画模式
	glLineWidth(5); //线宽
	glLineStipple(1, 0x00FF);
	glBegin(GL_LINES);

	GLfloat i = 0.0f;
	for (i = 0.06f; i <= 0.12f; i += 0.01f)
	{
		//上面
		glVertex3f(-1.6f, 0.15f, 0.05f);
		glVertex3f(0.0f, 0.3f, i);
		//下面
		glVertex3f(-1.6f, -0.15f, 0.05f);
		glVertex3f(0.0f, -0.3f, i);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	//Angle,Aspect Ratio,near plane,far plane 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


// 转为弧度
GLfloat convert_radians(GLfloat v) {
	return v * PI / 180.0f;
}
// 角度求和，注意边界
GLfloat angleSum(GLfloat a, GLfloat b) {
	a += b;
	if (a < 0) return a + 2 * PI;
	else if (a > 2 * PI) return a - 2 * PI;
	else return a;
}
// 转为角度
GLfloat convert_angle(GLfloat v) {
	return v * 180.0f / PI;
}

void update(void) {
	if (-0.05f < run_speed && run_speed < 0.05f) return;

	if (run_speed < 0.0f) { // 负值
		pedal_angle = run_speed = 0.0f;
	}
	GLfloat xDelta = 0.0f;
	GLfloat yDelta = 0.0f;
	xDelta = run_speed * cos(convert_radians(direction + toward));
	yDelta = run_speed * sin(convert_radians(direction + toward));

	position_x += xDelta;
	position_z -= yDelta;

	pedal_angle = convert_angle(angleSum(convert_radians(pedal_angle), run_speed / 1.0f));

	GLfloat rotation = 0.0f;
	rotation = atan2(run_speed * sin(convert_radians(toward)), 3.3f + run_speed * cos(convert_radians(toward)));
	direction = convert_angle(angleSum(convert_radians(direction), rotation));


}



// 画支柱  横向和竖向
void drawCylinder(GLfloat radius, GLfloat length, GLint direction) {
	if (direction == -1) {
		glPushMatrix();
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	}
	GLUquadricObj* cylinder;
	cylinder = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	
	gluCylinder(cylinder, radius, radius, length, 15, 5);
	glPopMatrix();

	if (direction == -1) {
		glPopMatrix();
	}
}

//画齿轮：用四边形模拟 齿轮内半径，外半径，高度，宽度，数量 案例：opengl画齿轮
void drawGearWheel(GLfloat inner_radius, GLfloat outer_radius, GLfloat height, GLfloat width, GLint number) {
	GLint i = 0;
	GLfloat r0 = 0.0f;
	GLfloat r1 = 0.0f;
	GLfloat r2 = 0.0f;
	GLfloat angle = 0.0f;
	GLfloat da = 0.0f;
	r0 = inner_radius;
	r1 = outer_radius - height / 2.0;
	r2 = outer_radius + height / 2.0;

	glShadeModel(GL_FLAT);

	glNormal3f(0.0, 0.0, 1.0);
	//前面的
	glBegin(GL_QUAD_STRIP);
	da = 2.0 * PI / number / 4.0;
	for (i = 0; i <= number; i++) {
		angle = i * 2.0 * PI / number;
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	//前面的
	glBegin(GL_QUADS);
	da = 2.0 * PI / number / 4.0;
	for (i = 0; i < number; i++) {
		angle = i * 2.0 * PI / number;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	glNormal3f(0.0, 0.0, -1.0);
	//画背面
	glBegin(GL_QUAD_STRIP);
	da = 2.0 * PI / number / 4.0;
	for (i = 0; i <= number; i++) {
		angle = i * 2.0 * PI / number;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
	}
	glEnd();
	glBegin(GL_QUADS);
	da = 2.0 * PI / number / 4.0;
	for (i = 0; i < number; i++) {
		angle = i * 2.0 * PI / number;
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
	}
	glEnd();

	GLfloat u = 0.0f;
	GLfloat v = 0.0f;
	GLfloat len = 0.0f;
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < number; i++) {
		angle = i * 2.0 * PI / number;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u * u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
		glNormal3f(v, -u, 0.0);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
	}

	glVertex3f(r1 * cos(0.0), r1 * sin(0.0), width * 0.5);
	glVertex3f(r1 * cos(0.0), r1 * sin(0.0), -width * 0.5);

	glEnd();


	glShadeModel(GL_SMOOTH);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= number; i++) {
		angle = i * 2.0 * PI / number;
		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	}
	glEnd();

}



//画自行车的大框架
void drawBicycleFrame(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.06f);
	glRotatef(-2 * pedal_angle, 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	//if (flag == true) {
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//}
	drawGearWheel(0.08f, 0.3f, 0.03f, 0.03f, 50); //画主齿轮
	
		
	glPopMatrix();

	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.2f);
	
	drawCylinder(0.08f, 0.32f, 0);
	

	glPopMatrix();

	glRotatef(58.0f, 0.0f, 0.0f, 1.0f); 
	drawCylinder(0.05f, 1.7f, -1);   // 主连接杠 上长杠

	glRotatef(48.0f, 0.0f, 0.0f, 1.0f);
	drawCylinder(0.05f, 1.6f, -1); // 副连接杠 上斜杠

	glColor3f(1.0f, 0.0f, 1.0f);
	glTranslatef(1.7f, 0.0f, 0.0f);
	glRotatef(-106.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.3f, 0.05f, 0.25f);
	drawSeat(); //座椅
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
	drawCylinder(0.05f, 0.5f, -1); // 下横杠 （合并横杠）

	glPushMatrix();
	glTranslatef(0.5f, 0.0f, 0.11f);
	drawCylinder(0.05f, 1.08f, -1); //横杠两侧 （横杠分支正面）
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, 0.0f, -0.11f);
	drawCylinder(0.05f, 1.08f, -1); //横杠两侧 （横杠分支反面）
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.58f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(-2 * pedal_angle, 0.0f, 0.0f, 1.0f);
	drawWheel(); // 画轮子 后轮

	glColor3f(0.0f, 0.0f, 1.0f);
	drawGearWheel(0.03f, 0.15f, 0.03f, 0.03f, 20);  // 副齿轮
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();
	glRotatef(50.0f, 0.0f, 0.0f, 1.0f);


	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.11f);
	drawCylinder(0.05f, 1.1f, -1);  // 副齿轮的连接斜杠 （斜杠分支正面）
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.11f);  // 副齿轮的连接斜杠 （斜杠分支反面）
	drawCylinder(0.05f, 1.1f, -1);
	glPopMatrix();

	glTranslatef(1.1f, 0.0f, 0.0f);
	drawCylinder(0.05f, 0.7f, -1);  // 斜杠 （合并副齿轮的连接斜杠）


	glTranslatef(0.7f, 0.0f, 0.0f);
	glRotatef(-50.0f, 0.0f, 0.0f, 1.0f);  
	drawCylinder(0.05f, 1.5f, -1);   // 上横杠

	glTranslatef(1.5f, 0.0f, 0.0f);
	glRotatef(-70.0f, 0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	drawCylinder(0.05f, 0.45f, -1); // 连接上横杠和上斜杠
	glPopMatrix();

	glPushMatrix();
	glRotatef(-toward, 1.0f, 0.0f, 0.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(70.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.6f);
	drawCylinder(0.05f, 1.2f, 1); // 车把
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.6f);
	drawCylinder(0.07f, 0.3f, 1);

	glTranslatef(0.0f, 0.0f, 0.9f);
	drawCylinder(0.07f, 0.3f, 1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	drawCylinder(0.05f, 0.7f, -1);

	glTranslatef(0.7f, 0.0f, 0.0f);
	glRotatef(8.0f, 0.0f, 0.0f, 1.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.11f);
	drawCylinder(0.05f, 1.12f, -1); // 前轮的连接杠 （正面）
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.11f);
	drawCylinder(0.05f, 1.12f, -1);   // 前轮的连接杠 （反面）
	glPopMatrix();

	glTranslatef(1.12f, 0.0f, 0.0f);
	glRotatef(-2 * pedal_angle, 0.0f, 0.0f, 1.0f);
	drawWheel(); // 画轮子 前轮
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

//初始化窗口内的场景
void init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);    //按指定颜色清理窗口
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);          //采用过渡的光照模式

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	glEnable(GL_LIGHTING);  //允许光照        
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL); //允许颜色材质
}


//绘制图元
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除缓冲区
	glEnable(GL_NORMALIZE); //法线

	glPushMatrix();

	//轴
	glRotatef(axisX_angle, 1.0f, 0.0f, 0.0f); //x
	glRotatef(axisY_angle, 0.0f, 1.0f, 0.0f); //y
	glRotatef(axisZ_angle, 0.0f, 0.0f, 0.0f); //z

	//画场景
	drawScene();

	glPushMatrix();
	glTranslatef(position_x, 0.0f, position_z);
	glRotatef(direction, 0.0f, 1.0f, 0.0f);
	if (flag == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //线框图
	} 
	//画自行车框架
	drawBicycleFrame();
	//画细节
	//链子
	drawChain(); 
	//脚踏板
	drawPedals();

	//轮子和座椅需要画在框架里面，不然位置不对


	glPopMatrix();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPopMatrix();
	gluLookAt(eye_x, eye_y, eye_z, eye_x, 0.0, 0.0, 0.0, 1.0, 0.0); //视图

	glutSwapBuffers(); //缓冲区

}

//画场景
void drawScene(void) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES); //画线
	GLfloat i = 0.0f;
	for (i = -100.0f; i < 100.0f; i += 1.0f)
	{
		glVertex3f(-100.0f, -1.0f, i);
		glVertex3f(100.0f, -1.0f, i);
		glVertex3f(i, -1.0f, -100.0f);
		glVertex3f(i, -1.0f, 100.0f);
	}
	glEnd();
}

void idle(void)
{
	update();
	menu_item(); // 菜单选择
	// update();
	glutPostRedisplay(); // 刷新

}

// 键盘操纵移动视角 https://www.jianshu.com/p/985e23cf03c6
void specialKey(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP:
			eye_z -= 0.1f;
			break;
		case GLUT_KEY_DOWN:
			eye_z += 0.1f;
			break;
		case GLUT_KEY_LEFT:
			eye_x -= 0.1f;
			break;
		case GLUT_KEY_RIGHT:
			eye_x += 0.1f;
			break;
	}
	glutPostRedisplay();
}

// 操作自行车  W：加速，pedal_angle改变，旋转的角度会改变，此时轮子就转动起来了
void keyBoard(unsigned char key, int x, int y) {

	switch (key) {
		case 'X':
			run_speed = 0.0f;
			break;
		case 'W':
			run_speed += 0.05f;
			break;
		case 'S':
			run_speed -= 0.05f;
			break;
		case 'A':
			if (toward < 70.0f) {
				toward += 2.0f;
			}
			break;
		case 'D':
			if (toward > -70.0f) {
				toward -= 2.0f;
			}
			break;
		case 'R':
			axisX_angle = 0.0f;
			axisY_angle = 0.0f;
			axisZ_angle = 0.0f;
			pedal_angle = 0.0f;
			toward = 0.0f;
			run_speed = 0.0f;
			position_x = 0.0f;
			position_z = 0.0f;
			direction = 0.0f;
			eye_x = 0.0f;
			eye_y = 0.0f;
			eye_z = 8.0f;
			break;
		default:
			break;
	}


	pedal_angle += run_speed;
	if (run_speed < 0.0f) {
		run_speed = 0.0f;
	}	
	if (pedal_angle < 0.0f) {
		pedal_angle = 0.0f;
	}
	if (pedal_angle >= 360.0f) {
		pedal_angle -= 360.0f;
	}
	glutPostRedisplay();
}

//处理菜单事件
void processMenuEvents(int id) {
	menu_id = id;  // 菜单的选项
	glutPostRedisplay(); //刷新
}

//创建菜单
void createMenu() {


	//创建运动速度菜单：高、中、低
	speedMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("高", 1);
	glutAddMenuEntry("中", 2);
	glutAddMenuEntry("低", 3);

	//创建填充图菜单：线框图、填充图
	fillMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("填充图", 5); //本身就是填充图
	glutAddMenuEntry("线框图", 4);


	//利用鼠标右键弹出2个2层菜单（子菜单）
	mainMenu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("运动速度", speedMenu);
	glutAddSubMenu("填充图", fillMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 右键点击显示菜单

}
// 菜单选项
void menu_item(void)
{
	switch (menu_id) {
		case 1: // 高
			run_speed = run_speed + 0.1f;
			break;
		case 2: //中
			run_speed = run_speed + 0.05f;
			break;
		case 3: //低
			run_speed = run_speed + 0.01f;
		case 4: // 线框图
			//cout << "===========================" << endl;
			cout << flag << endl;
			flag = true;
			break;
		case 5: // 填充图
			cout << flag << endl;
			flag = false;
			
			//glFlush();   //刷新窗口以显示当前绘制图形

			break;
		default:
			flag = false;
			break;
	}
	glutPostRedisplay();
}


//主函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // 初始化
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //设置显示模式
	glutInitWindowPosition(100, 100);//设置窗口在屏幕中的位置
	glutInitWindowSize(1400, 700);//设置窗口的尺寸
	glutCreateWindow("2021262767  孙义");//设置窗口标题

	init();                                 //初始化场景
	createMenu(); // 创建菜单
	glutDisplayFunc(display);    //显示
	glutReshapeFunc(reshape);
	glutIdleFunc(idle); // 全局回调函数
	glutSpecialFunc(specialKey); // 移动视角
	glutKeyboardFunc(keyBoard); // 控制加速或减速或停止
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutMainLoop(); // 主循环
	return 0;
}
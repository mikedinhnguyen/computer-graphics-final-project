
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

SnGroup* gCity;
SnGroup* gFalcon;
SnGroup* gCar;
SnGroup* gCar2;
SnGroup* gCar3;
SnGroup* gCar4;
SnGroup* gCar5;
SnGroup* gBuilding;
SnGroup* gBus;
SnGroup* gBus2;
SnGroup* gLeftArm;
SnGroup* gRightArm;
SnGroup* gLego;
SnGroup* gBallon;
SnGroup* gBalloon2;
SnGroup* gBalloon3;
SnGroup* gAirplane;
SnGroup* g;
SnTransform* tCity;
SnTransform* tFalcon;
SnTransform* tCar;
SnTransform* tCar2;
SnTransform* tCar3;
SnTransform* tCar4;
SnTransform* tCar5;
SnTransform* tBus;
SnTransform* tBus2;
SnTransform* tLeftArm;
SnTransform* tRightArm;
SnTransform* tLego;
SnTransform* tBalloon;
SnTransform* tBalloon2;
SnTransform* tBalloon3;
SnTransform* tAirplane;

GsMat rarmMat, larmMat;
GsMat balloon, balloon2, balloon3;
GsMat balloonUp, balloonUp2, balloonUp3;
GsMat leftArm, rightArm, bod;
//SnTransform* tBuilding;
float gtime;

GsMat m_rot;

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	background(GsColor(0.0f, 1.0f, 1.0f, 1.0f));
	build_ui ();
	build_scene ();
}

void MyViewer::build_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _nbut=new UiCheckButton ( "Normals", EvNormals ) ); 
	}
	p->add ( new UiButton ( "Animate", EvAnimate ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::add_model ( SnShape* s, GsVec p )
{
	// This method demonstrates how to add some elements to our scene graph: lines,
	// and a shape, and all in a group under a SnManipulator.
	// Therefore we are also demonstrating the use of a manipulator to allow the user to
	// change the position of the object with the mouse. If you do not need mouse interaction,
	// you can just use a SnTransform to apply a transformation instead of a SnManipulator.
	// You would then add the transform as 1st element of the group, and set g->separator(true).
	// Your scene graph should always be carefully designed according to your application needs.

	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation ( p );
	manip->initial_mat ( m );

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	// manip->visible(false); // call this to turn off mouse interaction

	rootg()->add(manip);
}

static int factorial(int n) {
	if (n == 0) 
		return 1;
	else return (n * factorial(n - 1));
}

static GsPnt bezier(float t, GsArray<GsPnt>& P) {
	GsPnt point;
	int n = P.size() - 1;
	for (int i = 0; i < P.size(); i++) {
		int C = factorial(n) / (factorial(i) * factorial(n - i));
		point += P[i] * float(C) * powf((1.0f - t), float(n - i)) * powf(t, (float)i);
	}
	return point;
}

static GsPnt CRSplines(float t, int i, GsArray<GsPnt>& P) {
	GsPnt I1 = (P[i + 1] - P[i - 1]) / 2;
	GsPnt I2 = (P[i + 2] - P[i]) / 2;
	GsPnt Pp = P[i] + (I1 / 3);
	GsPnt Pm = P[i + 1] + (I2 / 3);
	
	GsArray<GsPnt> temp; 
	temp.push() = P[i];
	temp.push() = Pp;
	temp.push() = Pm;
	temp.push() = P[i + 1];

	return bezier(t, P);
}

void MyViewer::build_scene ()
{

	GsMat rotate, trans;

	gCity = new SnGroup;
	gFalcon = new SnGroup;
	gCar = new SnGroup;
	gCar2 = new SnGroup;
	gCar3 = new SnGroup;
	gCar4 = new SnGroup;
	gCar5 = new SnGroup;
	gBus = new SnGroup;
	gBus2 = new SnGroup;
	gLeftArm = new SnGroup;
	gRightArm = new SnGroup;
	gLego = new SnGroup;
	gBallon = new SnGroup;
	gBalloon2 = new SnGroup;
	gBalloon3 = new SnGroup;
	gAirplane = new SnGroup;
	//gBuilding = new SnGroup;
	g = new SnGroup;


	//loading the lego model
	SnModel* mLeftArm = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mLeftArm->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/lego/lego-larm.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "its also there" << gsnl;
	}
	mLeftArm->model()->scale(0.5f);
	//mCar->model()->centralize();
	gLeftArm->separator(true);
	gLeftArm->add(tLeftArm = new SnTransform);
	gLeftArm->add(mLeftArm);
	trans.translation(GsVec(17.1f, 41.4f, 0.0f));
	tLeftArm->set(trans);

	SnModel* mRightArm = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mRightArm->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/lego/lego-rarm.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mRightArm->model()->scale(0.5f);
	//mCar->model()->centralize();
	gRightArm->separator(true);
	gRightArm->add(tRightArm = new SnTransform);
	gRightArm->add(mRightArm);
	trans.translation(GsVec(8.9f, 41.0f, 0.0f));
	tRightArm->set(trans);

	SnModel* mLego = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mLego->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/lego/lego-bod.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "its there";
	}
	mLego->model()->scale(0.5f);
	//mCar->model()->centralize();
	gLego->separator(true);
	gLego->add(tLego = new SnTransform);
	gLego->add(mLego);
	trans.translation(GsVec(13.0f, 38.0f, 0.0f));
	tLego->set(trans);


	//loading the bus model
	SnModel* mBus = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mBus->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/bus/bus.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mBus->model()->scale(3.0f);
	//mCar->model()->centralize();
	gBus->separator(true);
	gBus->add(tBus = new SnTransform);
	gBus->add(mBus);
	trans.translation(GsVec(150.0f, 25.5f, 15.0f));
	tBus->set(trans);

	//loading the 2nd bus model
	SnModel* mBus2 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mBus2->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/bus/bus.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mBus2->model()->scale(3.0f);
	//mCar->model()->centralize();
	gBus2->separator(true);
	gBus2->add(tBus2 = new SnTransform);
	gBus2->add(mBus2);
	trans.translation(GsVec(70.0f, 25.5f, -170.0f));
	tBus2->set(trans);
	rotate.roty((float)GS_2PI / -2.9f);
	tBus2->get().mult(trans, rotate);

	//loading the car model
	SnModel* mCar = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCar->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/car/car.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mCar->model()->scale(3.0f);
	//mCar->model()->centralize();
	gCar->separator(true);
	gCar->add(tCar = new SnTransform);
	gCar->add(mCar);
	trans.translation(GsVec(-20.0f, 21.5f, 55.0f));
	tCar->set(trans);


	//loading the car2 model
	SnModel* mCar2 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCar2->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/car/car.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mCar2->model()->scale(3.0f);
	//mCar->model()->centralize();
	gCar2->separator(true);
	gCar2->add(tCar2 = new SnTransform);
	gCar2->add(mCar2);
	trans.translation(GsVec(-30.0f, 21.5f, -60.0f));
	tCar2->set(trans);

	//loading the 3rd car model
	SnModel* mCar3 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCar3->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/car/car.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mCar3->model()->scale(3.0f);
	//mCar->model()->centralize();
	gCar3->separator(true);
	gCar3->add(tCar3 = new SnTransform);
	gCar3->add(mCar3);
	trans.translation(GsVec(-270.0f, 21.5f, -215.0f));
	tCar3->set(trans);
	rotate.roty((float)GS_2PI / 2.9f);
	tCar3->get().mult(trans, rotate);

	//loading the 4th car model
	SnModel* mCar4 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCar4->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/car/car.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mCar4->model()->scale(3.0f);
	//mCar->model()->centralize();
	gCar4->separator(true);
	gCar4->add(tCar4 = new SnTransform);
	gCar4->add(mCar4);
	trans.translation(GsVec(120.0f, 21.5f, -140.0f));
	tCar4->set(trans);
	rotate.roty((float)GS_2PI / -2.9f);
	tCar4->get().mult(trans, rotate);

	//loading the 5th car model
	SnModel* mCar5 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCar5->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/car/car.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	mCar5->model()->scale(3.0f);
	//mCar->model()->centralize();
	gCar5->separator(true);
	gCar5->add(tCar5 = new SnTransform);
	gCar5->add(mCar5);
	trans.translation(GsVec(-170.0f, 21.5f, -140.0f));
	tCar5->set(trans);
	rotate.roty((float)GS_2PI / 2.9f);
	tCar5->get().mult(trans, rotate);
	

	//loading the balloon 3 model
	SnModel* mBalloon3 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mBalloon3->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/balloon/Air_Balloon.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "balloon 3 is there" << gsnl;
	}
	//mBalloon->model()->scale(0.5f);
	//mCar->model()->centralize();
	gBalloon3->separator(true);
	gBalloon3->add(tBalloon3 = new SnTransform);
	gBalloon3->add(mBalloon3);
	trans.translation(GsVec(-289.1f, 106.4f, -140.0f));
	tBalloon3->set(trans);

	//loading the balloon model
	SnModel* mBalloon = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mBalloon->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/balloon/Air_Balloon.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "balloon is there" << gsnl;
	}
	//mBalloon->model()->scale(0.5f);
	//mCar->model()->centralize();
	gBallon->separator(true);
	gBallon->add(tBalloon = new SnTransform);
	gBallon->add(mBalloon);
	trans.translation(GsVec(9.1f, 120.4f, -130.0f));
	tBalloon->set(trans);

	//loading the balloon 2 model
	SnModel* mBalloon2 = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mBalloon2->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/balloon/Air_Balloon.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "balloon 2 is there" << gsnl;
	}
	//mBalloon->model()->scale(0.5f);
	//mCar->model()->centralize();
	gBalloon2->separator(true);
	gBalloon2->add(tBalloon2 = new SnTransform);
	gBalloon2->add(mBalloon2);
	trans.translation(GsVec(89.1f, 140.4f, -110.0f));
	tBalloon2->set(trans);

	//loading the city model
	SnModel* mCity = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mCity->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/city/sirus City.obj")) {
		gsout << "ha sucker" << gsnl;
		
	}
	mCity->model()->scale(0.5f);
	//rightPeddle->model()->centralize();
	gCity->separator(true);
	gCity->add(tCity = new SnTransform);
	gCity->add(mCity);
	//trans.translation(GsVec(0, 0.0f, 0));
	//tCity->set(trans);	

	//loading the falcon model
	SnModel* mFalcon = new SnModel;
	gsout << "check" << gsnl;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mFalcon->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/falcon/millenium-falcon.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	gsout << "check 2" << gsnl;
	mFalcon->model()->scale(0.10f);
	//mFalcon->model()->centralize();
	gFalcon->separator(true);
	gFalcon->add(tFalcon = new SnTransform);
	gFalcon->add(mFalcon);
	trans.translation(GsVec(0.0f, 250.0f, 0.0f));
	tFalcon->set(trans);

	m_rot.rotx(-(gspi / 2));

	//loading the airplane model
	SnModel* mAirplane = new SnModel;
	//rpX = 5.0f; rpY = -4.5f; rpZ = -3.5f;
	if (!mAirplane->model()->load("C:/Users/Lopez/OneDrive/Desktop/CSE/CSE170/FinalProject/sigapp/vs2019/airplane/11805_airplane_v2_L2.obj")) {
		gsout << "ha sucker" << gsnl;
	}
	else {
		gsout << "airplane is here" << gsnl;
	}
	mAirplane->model()->scale(0.07f);
	mAirplane->model()->centralize();
	gAirplane->separator(true);
	gAirplane->add(tAirplane = new SnTransform);
	gAirplane->add(mAirplane);
	trans.translation(GsVec(0.0f, 200.0f, 0.0f));
	tAirplane->set(trans);
	tAirplane->get().mult(tAirplane->get(), m_rot);

	gsout << "hello world";

	g->add(gFalcon);
	g->add(gCity);
	g->add(gCar);
	g->add(gCar2);
	g->add(gCar3);
	g->add(gCar4);
	g->add(gCar5);
	g->add(gBus);
	g->add(gBus2);
	g->add(gLeftArm);
	g->add(gRightArm);
	g->add(gLego);
	g->add(gBallon);
	g->add(gBalloon2);
	g->add(gBalloon3);
	g->add(gAirplane);
	//g->add(gBuilding);
	rootg()->add(g);


	GsArray<GsPnt> falconpnt;
	//falconpnt.push() = GsPnt(250.f, 250.f, 0.0f);
	//falconpnt.push() = GsPnt(-250.f, 250.f, 0.0f);
	//falconpnt.push() = GsPnt(-250.f, 250.f, 250.0f);
	//falconpnt.push() = GsPnt(250.f, 250.f, -250.0f);

	float r = 500.f;
	for (float theta = 0.0f; theta < gs2pi; theta += gs2pi /12.0f) {
		falconpnt.push() = GsPnt(r*cosf(theta), 250.f, r*sinf(theta));
	}

	//showMyPoints(falconpnt); // shows the points created.
	float deltat = 0.003f;
	for (int i = 1; i < falconpnt.size() - 2; i ++) {
		for(float t = 0.0f; t < 1.0f ; t+= deltat) {
			GsPnt P = CRSplines(t, i, falconpnt); // use CR splines
			falconPath.push() = P; // Creates the path
		}
	}
}

void MyViewer::showMyPoints(GsArray<GsPnt> P) {
	SnModel* sPoints;
	for (int i = 0; i < P.size(); i++) {
		sPoints = new SnModel;
		sPoints->model()->make_sphere(P[i], 20.0f, 50, true);
		sPoints->color(GsColor::red);
		rootg()->add(sPoints);
	}
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{
	//if ( _animating ) return; // avoid recursive calls
	//_animating = true;
	//
	//int ind = gs_random ( 0, rootg()->size()-1 ); // pick one child
	//SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	//GsMat m = manip->mat();

	//double frdt = 1.0/30.0; // delta time to reach given number of frames per second
	//double v = 4; // target velocity is 1 unit per second
	//double t=0, lt=0, t0=gs_time();
	//do // run for a while:
	//{	while ( t-lt<frdt ) { ws_check(); t=gs_time()-t0; } // wait until it is time for next frame
	//	double yinc = (t-lt)*v;
	//	if ( t>2 ) yinc=-yinc; // after 2 secs: go down
	//	lt = t;
	//	m.e24 += (float)yinc;
	//	if ( m.e24<0 ) m.e24=0; // make sure it does not go below 0
	//	manip->initial_mat ( m );
	//	render(); // notify it needs redraw
	//	ws_check(); // redraw now
	//}	while ( m.e24>0 );
	//_animating = false;

	double frdt = 1.0 / 30.0;
	double t, lt, t0 = gs_time();
	gtime = 0.0f;
	t = 0;
	lt = 0;
	int i = 0;

	do {
		lt = gs_time() - t0;
		if (lt < 4.0f) {
			//camera().eye.x -= 0.5f;
			//lightPos += GsVec(0.1f, 0, 0.1f);
			camera().eye.z -= 1.5f;
			camera().eye.y += 0.8f;
			//camera().center.y += 0.05f;
			//update1 = true;
		}
		if (lt > 4.0f && lt <= 8.5f) {
			camera().eye.x += 1.5f;
			camera().eye.z -= 1.5f;
			//camera().eye.y += 0.05f;
		}
		if (lt > 8.5f && lt <= 12.0f) {
			/*camera().eye.x -= 1.0f;
			camera().eye.z -= 1.0f;*/
			camera().eye.x -= 1.0f;
			camera().eye.z -= 1.0f;
			camera().eye.y -= 0.05f;
		}
		if (lt > 12.0f && lt <= 16.0f) {
			camera().eye.y -= 0.5f;
			camera().eye.x -= 1.0f;
		}
		if (lt > 16.0f && lt <= 22.0f) {
			camera().eye.z += 1.0f;
		}
		if (lt > 22.0f && lt <= 26.0f) {
			camera().eye.z -= 0.05f;
			camera().eye.x += 1.0f;
		}
		if (lt > 26.0f && lt <= 30.0f) {
			camera().eye.x += 1.0f;
		}
		if (lt > 30.0f && lt <= 32.0f) {
			camera().eye.z -= 1.0f;
			camera().eye.y += 0.1f;
		}
		if (lt > 32.0f && lt <= 40.0f) {
			camera().eye.z += 0.5f;
			camera().eye.x -= 1.0f;
		}
		if (lt > 40.0f && lt <= 43.0f) {
			camera().eye.y -= 1.0f;
			camera().eye.x -= 0.0001f;
			camera().eye.z -= 0.0001f;
		}
		render();
		ws_check();
		message().setf("local time = % f", lt);
	} while (lt <= 43.0f);

	//do {
	//	lt = gs_time() - t0;
	//	if (lt < 2.0f) {
	//		camera().eye.x += 0.5f;
	//		//lightPos += GsVec(0.1f, 0, 0.1f);
	//		camera().eye.z += 0.5f;
	//		camera().center.y = 0.0f;
	//		//update1 = true;
	//	}
	//	if (lt > 2.0f && lt <= 5.0f) {
	//		camera().eye.x -= 1.0f;
	//		camera().eye.z -= 1.0f;
	//	}
	//	if (lt > 5.0f && lt <= 10.0f) {
	//		camera().eye.x -= 1.0f;
	//		camera().eye.z -= 1.0f;
	//	}
	//	if (lt > 10.0f && lt <= 15.0f) {
	//		camera().eye.z -= 1.0f;
	//		camera().eye.y += 0.5f;
	//	}
	//	render();
	//	ws_check();
	//	message().setf("local time = % f", lt);
	//} while (lt <= 15.0f);

	//do {
	//	while (t - lt < frdt) { ws_check(); t = gs_time() - t0; }
	//	if (i < falconPath.size()) {
	//		tFalcon->get().translation(falconPath[i]);
	//	}
	//	else {
	//		i = 0;
	//	}
	//	i++;
	//	lt = t;
	//	render(); // notify it needs redraw
	//	ws_check(); // redraw now
	//} while (true);
	
	

}

//void MyViewer::run_lego() {
//	if (_lego) return;
//	_lego = true;
//	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
//	double t1 = 0, lt = 0, t0 = gs_time();
//
//	do {
//		while (t1 - lt < frdt) { ws_check(); t1 = gs_time() - t0; } // wait until it is time for next frame
//		lt = t1;
//
//		rarmMat.rotx(0.1);
//		tRightArm->get().mult(tRightArm->get(), rarmMat);
//		larmMat.rotx(-0.1);
//		tLeftArm->get().mult(tLeftArm->get(), larmMat);
//
//		double frdt = 1.0 / 30.0;
//		double t, lt, t0 = gs_time();
//		render();
//		ws_check();
//	} while (_lego);
//}

void MyViewer::run_car()
{
	if (_car) return; // avoid recursive calls
	_car = true;
	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double t1 = 0, lt = 0, t0 = gs_time();
	GsMat rotate, trans;
	float x = 0;
	float y = 0.2f;
	float z = 0;
	float a = 0;
	float b = 0;
	float c = 0;
	float d = 0;
	int i = 0;
	do {
		while (t1 - lt < frdt) { ws_check(); t1 = gs_time() - t0; } // wait until it is time for next frame
		lt = t1;

		rotate.translation(0, 0, x);
		trans = tCar->get();
		tCar->get().mult(trans, rotate);
		rotate.translation(0, 0, y);
		trans = tCar2->get();
		tCar2->get().mult(trans, rotate);
		rotate.translation(0, 0, z);
		trans = tBus->get();
		tBus->get().mult(trans, rotate);
		rotate.translation(0, 0, a);
		trans = tCar3->get();
		tCar3->get().mult(trans, rotate);
		rotate.translation(0, 0, b);
		trans = tCar4->get();
		tCar4->get().mult(trans, rotate);
		rotate.translation(0, 0, c);
		trans = tCar5->get();
		tCar5->get().mult(trans, rotate);
		rotate.translation(0, 0, d);
		trans = tBus2->get();
		tBus2->get().mult(trans, rotate);
		render();
		ws_check();
		x = x + 0.001f;
		y = y + 0.002f;
		z = z + 0.0015f;
		a = a + 0.0015f;
		b = b + 0.002f;
		c = c + 0.0025f;
		d = d + 0.0015f;
		if (x > 0.435f) {
			x = 0.2f;
			rotate.roty((float)GS_2PI / 4.0f);
			trans = tCar->get();
			tCar->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (y > 0.35f) {
			y = 0.1f;
			rotate.roty((float)GS_2PI / -4.0f);
			trans = tCar2->get();
			tCar2->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (z > 0.5f) {
			z = 0;
			rotate.roty((float)GS_2PI / 4.0f);
			trans = tBus->get();
			tBus->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (a > 0.45f) {
			a = 0;
			rotate.roty((float)GS_2PI / -4.0f);
			trans = tCar3->get();
			tCar3->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (b > 0.45f) {
			b = 0;
			rotate.roty((float)GS_2PI / -4.0f);
			trans = tCar4->get();
			tCar4->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (c > 0.65f) {
			c = 0;
			rotate.roty((float)GS_2PI / 4.0f);
			trans = tCar5->get();
			tCar5->get().mult(trans, rotate);
			render();
			ws_check();
		}
		if (d > 0.57f) {
			d = 0;
			rotate.roty((float)GS_2PI / -4.0f);
			trans = tBus2->get();
			tBus2->get().mult(trans, rotate);
			render();
			ws_check();
		}

		if (i < falconPath.size() -1) {
			GsMat trans, rot;
			float angleYfalcon = atan2f((falconPath[i].x - falconPath[i + 1].x), (falconPath[i].z - falconPath[i + 1].z));
			float angleYplane = atan2f((falconPath[falconPath.size() - i].x - falconPath[falconPath.size() - i - 1].x), (falconPath[falconPath.size() - i].z - falconPath[falconPath.size() - i - 1].z));

			trans.translation(falconPath[i]);
			rot.roty(angleYfalcon);

			tFalcon->get() = trans * rot * trans.inverse() * trans;
			rot.roty(angleYplane);
			trans.translation(falconPath[falconPath.size()-i]);
			tAirplane->get() = trans * rot * m_rot * trans.inverse() * trans;
		}
		else {
			i = 0;
		}
		i++;
		lt = t1;
		render(); // notify it needs redraw
		ws_check(); // redraw now

		leftArm.translation((0.5f) * GsVec::k);
		rightArm.translation((0.5f) * GsVec::k);
		rarmMat.rotx(0.1f);
		tRightArm->get().mult(tRightArm->get(), rarmMat);
		//tRightArm->get().mult(tRightArm->get(), rightArm);
		larmMat.rotx(-0.1f);
		tLeftArm->get().mult(tLeftArm->get(), larmMat);

		balloonUp.translation((1.0f) * GsVec::j);
		balloon.roty(1.0f);
		tBalloon->get().mult(tBalloon->get(), balloon);
		tBalloon->get().mult(tBalloon->get(), balloonUp);

		balloonUp2.translation((0.8f) * GsVec::j);
		balloon2.roty(-1.0f);
		tBalloon2->get().mult(tBalloon2->get(), balloon2);
		tBalloon2->get().mult(tBalloon2->get(), balloonUp2);

		balloonUp3.translation((0.8f) * GsVec::j);
		balloon3.roty(-1.0f);
		tBalloon3->get().mult(tBalloon3->get(), balloon3);
		tBalloon3->get().mult(tBalloon3->get(), balloonUp3);

	} while (_car);
}

void MyViewer::show_normals ( bool view )
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for ( int k=0; k<r->size(); k++ )
	{	SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if ( !view ) { l->visible(false); continue; }
		l->visible ( true );
		if ( !l->empty() ) continue; // build only once
		l->init();
		if ( s->instance_name()==SnPrimitive::class_name )
		{	GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face ( fn );
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for ( int i=0; i<m.F.size(); i++ )
			{	const GsVec& a=m.V[m.F[i].a]; l->push ( a, a+(*n++)*f );
				const GsVec& b=m.V[m.F[i].b]; l->push ( b, b+(*n++)*f );
				const GsVec& c=m.V[m.F[i].c]; l->push ( c, c+(*n++)*f );
			}
		}  
	}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	//double frdt = 1.0 / 30.0;
	//double t, lt, t0 = gs_time();
	//gtime = 0.0f;
	//t = 0;
	//lt = 0;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }

		//Camera goes around the map showing how the map looks like
		case 'r': {
			if (_car)_car = false;
			else run_car();
			return 1;
		}

		//security camera 1: cars
		case '1': {
			//camera().eye.x = 20.0f;
			camera().eye.x = 0.0f;
			camera().eye.z = 0.0f;
			camera().center.y = 12.0f;
			ws_check();
			render();
			break;
		}

		//security camera 2: lego man
		case '2': {
			camera().eye.x = 25.0f;
			camera().eye.z = 25.0f;
			camera().eye.y = 50.0f;
			ws_check();
			render();
			break;
		}

		//security camera 3: bus
		case '3': {
			//camera().eye.x = 20.0f;
			camera().eye.x = 210.0f;
			camera().eye.z = 90.0f;
			camera().center.y = 15.0f;
			ws_check();
			render();
			break;
		}

		//security camera 4: looking at the sky
		case '4': {
			//camera().eye.x = 20.0f;
			camera().eye.x = 10.0f;
			camera().eye.z = 20.0f;
			camera().center.y = 150.0f;
			ws_check();
			render();
			break;
		}

		//security camera 5: aricrafts
		case '5': {
			//camera().eye.x = 20.0f;
			camera().eye.x = 20.0f;
			camera().eye.z = 200.0f;
			camera().eye.y = 700.0f;
			ws_check();
			render();
			break;
		}
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvNormals: show_normals(_nbut->value()); return 1;
		case EvAnimate: run_animation(); return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}

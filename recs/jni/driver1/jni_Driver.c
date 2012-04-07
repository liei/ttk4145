 #include "jni_Driver.h"
 #include "elev.h"

/****************************************************************\
|*                     Callback Functions                       *|
|*                                                              *|
\****************************************************************/

static int elev_threadid;

static JavaVM *g_vm = NULL;
static jobject g_this;

void java_callback(jmethodID mid, int floor, int value){
  JNIEnv *env;
  if ((*g_vm)->AttachCurrentThread(g_vm, (void **) &env, NULL) < 0){
	fprintf(stderr, "%s[%d]: AttachCurrentThread ()\n",__FILE__, __LINE__);
  }
  
  (*env)->CallVoidMethod(env, g_this, mid,floor,value);
  
  if ((*g_vm)->DetachCurrentThread(g_vm) < 0){
	fprintf (stderr, "%s[%d]: DetachCurrentThread ()\n",__FILE__, __LINE__);
  }
}

#define NUM_CALLBACKS 4
#define CALLBACK_BUTTONS 0
#define CALLBACK_SENSOR 1
#define CALLBACK_STOP 2
#define CALLBACK_OBSTR 3

static jmethodID callbacks[NUM_CALLBACKS];
static char *callback_names[] = {"callback_buttons","callback_sensor","callback_stop","callback_obstruction"};

void elev_buttons(int floor, int value){
  java_callback(callbacks[CALLBACK_BUTTONS],floor,value);
}

void elev_sensor(int floor, int value){
  java_callback(callbacks[CALLBACK_SENSOR],floor,value);
}

void elev_stop(int floor, int value){
  java_callback(callbacks[CALLBACK_STOP],floor,value);
}

void elev_obstr(int floor, int value){
  java_callback(callbacks[CALLBACK_OBSTR],floor,value);
}

void setup_callback_functions(JNIEnv *env, jobject this){
  if((*env)->GetJavaVM(env, &g_vm) < 0){
	fprintf(stderr, "%s[%d]: GetJavaVM ()\n",__FILE__, __LINE__);
	return;
  } 

  g_this = (*env)->NewGlobalRef(env, this);
  if(g_this == NULL){
	fprintf (stderr, "%s[%d]: NewGlobalRef ()\n",__FILE__, __LINE__);
	return;
  }

  jclass class = (*env)->GetObjectClass(env,this);

  for(int i = 0; i < NUM_CALLBACKS; i++){
	callbacks[i] = (*env)->GetMethodID(env, class, callback_names[i], "(II)V");
	if (callbacks[i] == NULL){
	  fprintf (stderr, "%s[%d]: GetMethodID()\n",__FILE__, __LINE__);
	  return;
	}
  }
}

/****************************************************************\
|*                       JNI Functions                          *|
|*                                                              *|
\****************************************************************/

 /*
  * Class:     jni_driver_Driver
  * Method:    elev_init
  * Signature: ()I
  */
JNIEXPORT jint JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1init
(JNIEnv *env, jobject this){
  int err = elev_init();
  if(err != 0){
	return err;
  }
  
  printf("c    :: init\n");
  fflush(stdout);
  setup_callback_functions(env,this);

  // register callbacks
  elev_register_callback(SIGNAL_TYPE_CALL_UP,&elev_buttons);
  elev_register_callback(SIGNAL_TYPE_CALL_DOWN,&elev_buttons);
  elev_register_callback(SIGNAL_TYPE_COMMAND,&elev_buttons);
  elev_register_callback(SIGNAL_TYPE_SENSOR,&elev_sensor);
  elev_register_callback(SIGNAL_TYPE_STOP,&elev_stop);
  elev_register_callback(SIGNAL_TYPE_OBSTR,&elev_obstr);

  // start the callback thread
  elev_threadid = elev_enable_callbacks();

  return 0;
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1stop
(JNIEnv *env, jobject this){
  elev_disable_callbacks(elev_threadid);
  (*env)->DeleteGlobalRef(env,g_this);
}


/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setSpeed
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setSpeed
(JNIEnv *env, jobject this, jint speed){
  elev_set_speed(speed);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setDoorOpenLamp
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setDoorOpenLamp
(JNIEnv *env, jobject this, jint value){
  elev_set_door_open_lamp(value);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setStopLamp
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setStopLamp
(JNIEnv *env, jobject this, jint value){
  elev_set_stop_lamp(value);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setFloorIndicator
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setFloorIndicator
(JNIEnv *env, jobject this, jint floor){
  elev_set_floor_indicator(floor);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setButtonLamp
 * Signature: (III)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setButtonLamp
(JNIEnv *env, jobject this, jint lamp, jint floor, jint value){
  elev_set_button_lamp(lamp,floor,value);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_resetAllLamps
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1resetAllLamps
(JNIEnv *env, jobject this){
  elev_reset_all_lamps();
}

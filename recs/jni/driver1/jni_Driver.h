/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class edu_ntnu_ttk4145_recs_driver_Driver */

#ifndef _Included_edu_ntnu_ttk4145_recs_driver_Driver
#define _Included_edu_ntnu_ttk4145_recs_driver_Driver
#ifdef __cplusplus
extern "C" {
#endif
#undef edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_UP
#define edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_UP 0L
#undef edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_DOWN
#define edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_DOWN 1L
#undef edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_COMMAND
#define edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_COMMAND 2L
#undef edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_NONE
#define edu_ntnu_ttk4145_recs_driver_Driver_ELEV_DIR_NONE 2L
/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1init
  (JNIEnv *, jobject);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1stop
  (JNIEnv *, jobject);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setSpeed
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setSpeed
  (JNIEnv *, jobject, jint);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setDoorOpenLamp
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setDoorOpenLamp
  (JNIEnv *, jobject, jint);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setStopLamp
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setStopLamp
  (JNIEnv *, jobject, jint);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setFloorIndicator
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setFloorIndicator
  (JNIEnv *, jobject, jint);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_setButtonLamp
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1setButtonLamp
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_resetAllLamps
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1resetAllLamps
  (JNIEnv *, jobject);

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    elev_getFloorSensorSignal
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_edu_ntnu_ttk4145_recs_driver_Driver_elev_1getFloorSensorSignal
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

#include <comedilib.h>
#include "jni_io.h"

#define PORT0 edu_ntnu_ttk4145_recs_driver_Driver_PORT0
#define PORT1 edu_ntnu_ttk4145_recs_driver_Driver_PORT1
#define PORT2 edu_ntnu_ttk4145_recs_driver_Driver_PORT2
#define PORT3 edu_ntnu_ttk4145_recs_driver_Driver_PORT3
#define PORT4 edu_ntnu_ttk4145_recs_driver_Driver_PORT4


static comedi_t *it_g = NULL;

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1init
(JNIEnv *env, jobject this){
  int i, status = 0;

  it_g = comedi_open("/dev/comedi0");
  if (it_g == NULL)
	return 0;
  
  for (i = 0; i < 8; i++) {
	// comedi_dio_config is supposed to return 1 on success and -1 on error,
	// but seems to return 0 on success on newest versions. Anyway, do a
	// bitwise or, so that a single -1 will result in final value of -1.
	status |= comedi_dio_config(it_g, PORT1, i, COMEDI_INPUT);
	status |= comedi_dio_config(it_g, PORT2, i, COMEDI_OUTPUT);
	status |= comedi_dio_config(it_g, PORT3, i+8, COMEDI_OUTPUT);
	status |= comedi_dio_config(it_g, PORT4, i+16, COMEDI_INPUT);
  }
  return (status != -1);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1close
(JNIEnv *env, jobject this){
  return comedi_close(it_g);
}


/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_read_bit
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1read_1bit
(JNIEnv *env, jobject this, jint channel){
  unsigned int data = 0;
  comedi_dio_read(it_g, channel>>8, channel&0xff, &data);

  return (int) data;
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_set_bit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1set_1bit
(JNIEnv *env, jobject this, jint channel){
  comedi_dio_write(it_g, channel>>8,channel&0xff, 1);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_clear_bit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1clear_1bit
(JNIEnv *env, jobject this, jint channel){
  comedi_dio_write(it_g, channel>>8,channel&0xff, 0);
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_read_analog
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1read_1analog
(JNIEnv *env, jobject this, jint channel){
  lsampl_t data = 0;
  comedi_data_read(it_g, channel>>8, channel&0xff, 0, AREF_GROUND, &data);

  return (int)data;
}

/*
 * Class:     edu_ntnu_ttk4145_recs_driver_Driver
 * Method:    io_write_analog
 * Signature: (II)V
 */
JNIEXPORT void JNICALL 
Java_edu_ntnu_ttk4145_recs_driver_Driver_io_1write_1analog
(JNIEnv *env, jobject this, jint channel, jint data){
  comedi_data_write(it_g, channel>>8, channel&0xff, 0, AREF_GROUND, data);
}

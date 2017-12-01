#include "stdafx.h"
#include "JNIMain.h"
#include "string.h"
#include <Windows.h>



JNIEXPORT jstring JNICALL Java_JNIMain_getStringFromC
(JNIEnv* env, jclass jclz) {
	return (*env)->NewStringUTF(env, "C string");
};

JNIEXPORT jstring JNICALL Java_JNIMain_getStringFromC2
(JNIEnv *env, jobject jobj) {
	return (*env)->NewStringUTF(env,"C string2");

};
//���ʷǾ�̬��  
JNIEXPORT jstring JNICALL Java_JNIMain_accessField
(JNIEnv *env, jobject jobj) {
	//jclass 
	jclass jclz = (*env)->GetObjectClass(env, jobj);

	jfieldID fid = (*env)->GetFieldID(env, jclz, "key", "Ljava/lang/String;");
	//�õ�key��Ӧ��ֵ  ����getXXXField();  
	jstring jstr = (*env)->GetObjectField(env, jobj, fid);

	char *c_str = (*env)->GetStringUTFChars(env,jstr,NULL);

	char text[30] = "dongNao";

	strcat(text,c_str);
	//c_>jni
	jstring new_str = (*env)->NewStringUTF(env, text);
	(*env)->SetObjectField(env, jobj, fid, new_str);
	//�ͷ��ڴ�ռ�  
	(*env)->ReleaseStringChars(env, new_str, c_str);
	return new_str;

};

JNIEXPORT void JNICALL Java_JNIMain_accdssIntStaticField
(JNIEnv *env, jobject jobj) {
	//jclass 
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//fieldId ��������,����ǩ��  
	jfieldID fid = (*env)->GetStaticFieldID(env,jclz,"count","I");
	if (fid == NULL) {
		printf("fid is null");
	}
	jint count = (*env)->GetStaticIntField(env, jclz,fid);


	count++;

	(*env)->SetStaticIntField(env, jclz, fid, count);

};

//JNI����Java�еķǾ�̬�ķ���
JNIEXPORT void JNICALL Java_JNIMain_accessMethod
(JNIEnv *env, jobject jobj) {
	//��һ���ҵ�jclass
	jclass jclz = (*env)->GetObjectClass(env, jobj);
	//�ڶ����ҵ�jMethodId  ���ϵ��ĸ������Ƿ�����ǩ��
	jmethodID mid = (*env)->GetMethodID(env, jclz, "getRandomInt", "(I)I");
	//����  
	jint random = (*env)->CallIntMethod(env, jobj, mid, 200);

	printf("C random :%d", random);
};
//JNI����java�еľ�̬����  
JNIEXPORT void JNICALL Java_JNIMain_accessStaticMethod
(JNIEnv *env, jobject jobj) {
//jclass  ����ͨ��jobject ����class  ����ҵ��˽�class  ת���jclass���� 
	jclass jclz=(*env)->GetObjectClass(env,jobj);
	//jmethodId     
	jmethodID mid = (*env)->GetStaticMethodID(env,jclz,"getRandeomUUId","()Ljava/lang/String;");


	//���þ�̬����
	jstring uuid=(*env)->CallStaticObjectMethod(env, jclz, mid);
	  
	//����jstringת��Ϊchar*
	char *uuid_c = (*env)->GetStringUTFChars(env,uuid,NULL);

	//uuid.txt
	char filename[100];
	sprintf(filename,"D://%s.txt",uuid_c);

	FILE *fp = fopen(filename,"w");
	fputs("I Love A", fp);
	fclose(fp);
	printf("\n�ļ�д��ɹ�");
};

//JNI����Java�еĹ��췽��
JNIEXPORT jobject JNICALL Java_JNIMain_accessConstructor
(JNIEnv *env, jobject jobj) {
	//Ϊʲôʹ��findClass    ͨ�����·�� ��jvm���ҵ���Ӧ����  
	jclass jclz = (*env)->FindClass(env, "java/util/Date");

	//jmethodId  �ط�������ʾ����˼��  ���еĹ��췽�������ֶ���  <init>
	jmethodID mid = (*env)->GetMethodID(env, jclz, "<init>", "()V");

	//����newObject ʵ����Date����,����ֵ��һ��jobject
	jobject date_obj=(*env)->NewObject(env, jclz, mid);
	//ʹ�÷��������ҵ�ID
	jmethodID jmid=(*env)->GetMethodID(env, jclz, "getTime", "()J");

	jlong time=(*env)->CallLongMethod(env, date_obj, jmid);

	printf("time:%lld\n",time);
	return date_obj;

};
//JNIEXPORT jobject JNICALL Java_JNIMain_chineseChar
//(JNIEnv *env, jobject jobj,jstring in) {
//	jboolean iscp;
//	//�����������������õ����ֵ�� ��������  �������String�Ƿ������¿��ٵ��ڴ�    �������������Ƿ����¿��ٿ��ڴ� 
//	char *c_str = (*env)->GetStringChars(env, in, &iscp);
//
//	if (iscp == JNI_TRUE) {
//		printf(" is copy:JNI_TRUE\n");
//	}
//	else if (iscp == JNI_FALSE) {
//		printf("is copy :JNI_FALSE\n");
//	}
//	//
//	int length = (*env)->GetStringLength(env, in);
//	const jchar *jcstr = (*env)->GetStringChars(env, in, NULL);
//	if (jcstr == NULL){
//		return NULL;
//	}
//
//	//jchar ->char    ��������������Ҫע��ĵط�  ��һ�����������㹻���ڴ�  �ڶ��� ���������������ڴ���г�ʼ��
//	char *rtn = (char *)malloc(sizeof(char)*length*2 + 1);
//	memset(rtn, 0, sizeof(char)*length*2 + 1);
//	int size = 0;
//	size=WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)jcstr, length, rtn, sizeof(char)*length*2 + 1, NULL, NULL);
//
//	/*if (size <= 0) {
//		printf("size :0");
//		return NULL;
//	}*/
//    if(rtn!=NULL){
//       free(rtn);
//       rtn == NULL;
//                 }
// (*env)->ReleaseSrtingChars(env, in, c_str);
//
//	
//	//��java�л������������
//	printf("string:%s", rtn);
//
//	//ֻ�е�iscp��ture ��ʱ��  ���ǲ���Ҫ�ͷ��ڴ�  
//	//(*env)->ReleaseStringUTFChars(env,in,c_str);
//	return NULL;
//};

JNIEXPORT jobject JNICALL Java_JNIMain_chineseChar
(JNIEnv *env, jobject jobj, jstring in) {
	char *c_str = "̫��������";
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	//�Է�����ǩ��  ���ǿ�����javap��������
	jmethodID jmid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");

	//jstring ->jbyte
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(c_str));

	//��char *  ��ֵ��bytes   ��c_str����ת�� ת����jbyteArray   

	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(c_str), c_str);

	jstring chasetName = (*env)->NewStringUTF(env, "GB2312");

	return (*env)->NewObject(env, str_cls, jmid, bytes, chasetName);

};

//JNI�з�������(������������)
int compare(jint *a, jint *b);

JNIEXPORT void JNICALL Java_JNIMain_giveArray
(JNIEnv *env, jobject jobj, jintArray arr) {
	//��jintArray -> jint *  jvm  ����ݵ�ǰ�����  ������߲�����һ���ڴ� 
	jint *elemets=(*env)->GetIntArrayElements(env, arr, NULL);
	//���ڴ治����ʱ���Ϊ��
	if (elemets == NULL) {
		return;
	}
	//���鳤��
	int len = (*env)->GetArrayLength(env, arr);

	qsort(elemets,len,sizeof(jint),compare);

	(*env)->ReleaseIntArrayElements(env, arr, elemets, JNI_COMMIT);

	//�����д������һ�� ��ôjava�в���õ�һ�����  ��һ������������  1.�ͷ��п��ܵĻ�������   
	//2.������jni���޸ĵ����ݴ���д��ԭ�����ڴ�  ���Ե�����û�е����������ʱ�� ,���ǵò�����������  

};
int compare(jint *a,jint *b){
	return *a - *b;

}

//�����������͵�����
JNIEXPORT jobjectArray JNICALL Java_JNIMain_initStringArray
(JNIEnv *env, jobject jobj, jint size) {
   //����jobjectArray
	int i;
	jobjectArray result  ;
	jclass jclz = (*env)->FindClass(env,"java/lang/String");
		if (jclz == NULL) {
			return;
		}
	result=(*env)->NewObjectArray(env, size, jclz, jobj);
	if (result == NULL) {
		return NULL;
	}



		//��ֵ
	for (i = 0; i < size; i++) {
		char *c_str = (char*)malloc(256);
		memset(c_str, 0, 256);
		//��intת����Ϊchar  
		sprintf(c_str, "hello num:%d\n", i);
	

	//c-->jstring
	jstring str = (*env)->NewStringUTF(env,c_str);

	if (str == NULL){
		return NULL;
	}

//	��jstring��ֵ������
//����jobjectArray

	(*env)->SetObjectArrayElement(env, result, i, str);
	free(c_str);
	c_str = NULL;
	}

	return result;
};

//JNI����

//�ֲ�����    
//���巽ʽ����  :FindClass    NewObject   GetObjectClass  NewCharArray....  NewLocalFRef   ������jni�㴴����
//�����ٶ��߳���ʹ�� 
JNIEXPORT jobjectArray JNICALL Java_JNIMain_localRef
(JNIEnv *env, jobject jobj) {
	int i = 0;
	for (i = 0; i < 5; i++) {
		jclass cls = (*env)->FindClass(env, "java/util/Date");
		jmethodID mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
		//������һ��Date ���͵ľֲ�����
		jobject obj = (*env)->NewObject(env, cls, mid);

		//ʹ���������

		//�ͷ��������   ���û�е�������������  ��ôjvm  ���Զ��ͷŵ�   ֮������Ҫ�ֶ��ͷŵ�ԭ��ܼ� 
		(*env)->DeleteLocalRef(env, cls);
		(*env)->DeleteLocalRef(env, obj);
	}

	return NULL;
};

//ȫ������  
//��ǰ�� �緽�� 
jstring global_str;
JNIEXPORT jobjectArray JNICALL Java_JNIMain_createGlobalRef
(JNIEnv *env, jobject jobj) {
	jobject obj = (*env)->NewStringUTF(env, "JNI is intersting");
	//������һ��ȫ������
	global_str=(*env)->NewGlobalRef(env, obj);


};
JNIEXPORT jobjectArray JNICALL Java_JNIMain_getGlobalRef
(JNIEnv *env, jobject jobj) {
	return global_str;

};
JNIEXPORT jobjectArray JNICALL Java_JNIMain_delectGlobalRef
(JNIEnv *env, jobject jobj) {
	(*env)->DeleteGlobalRef(env, global_str);

};


//��ȫ������  
//��������ֹGC  ֻҪ�ڴ治�� �Ϳ���GC 
jclass  g_weak_cls;
JNIEXPORT jobjectArray JNICALL Java_JNIMain_createWeakRef
(JNIEnv *env, jobject jobj) {
	jclass cls_string = (*env)->FindClass(env, "java/lang/String");

	g_weak_cls = (*env)->NewWeakGlobalRef(env, cls_string);
	return g_weak_cls;


};


//JNI�쳣���� 

JNIEXPORT jstring JNICALL Java_JNIMain_exception
(JNIEnv *env, jobject jobj) {
	jclass cls = (*env)->GetObjectClass(env, jobj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "ksey", "Ljava/lang/String;");
	//����Ƿ����쳣
	jthrowable ex=(*env)->ExceptionOccurred(env);
	//�ж�ʱ�����쳣
	if (ex != NULL) {
		//���JNI�������쳣
		(*env)->ExceptionClear(env);
		jclass newExc=(*env)->FindClass(env,"java/lang/ILl");

	}
	//����Ļ�����쳣,,������ǻ��ӡ������ �����java���ǲ�һ����  java��Ҳ���ܲ���JNI���쳣  
	printf("exception");
};
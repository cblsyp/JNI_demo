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
//访问非静态域  
JNIEXPORT jstring JNICALL Java_JNIMain_accessField
(JNIEnv *env, jobject jobj) {
	//jclass 
	jclass jclz = (*env)->GetObjectClass(env, jobj);

	jfieldID fid = (*env)->GetFieldID(env, jclz, "key", "Ljava/lang/String;");
	//得到key对应的值  采用getXXXField();  
	jstring jstr = (*env)->GetObjectField(env, jobj, fid);

	char *c_str = (*env)->GetStringUTFChars(env,jstr,NULL);

	char text[30] = "dongNao";

	strcat(text,c_str);
	//c_>jni
	jstring new_str = (*env)->NewStringUTF(env, text);
	(*env)->SetObjectField(env, jobj, fid, new_str);
	//释放内存空间  
	(*env)->ReleaseStringChars(env, new_str, c_str);
	return new_str;

};

JNIEXPORT void JNICALL Java_JNIMain_accdssIntStaticField
(JNIEnv *env, jobject jobj) {
	//jclass 
	jclass jclz = (*env)->GetObjectClass(env,jobj);

	//fieldId 属性名称,属性签名  
	jfieldID fid = (*env)->GetStaticFieldID(env,jclz,"count","I");
	if (fid == NULL) {
		printf("fid is null");
	}
	jint count = (*env)->GetStaticIntField(env, jclz,fid);


	count++;

	(*env)->SetStaticIntField(env, jclz, fid, count);

};

//JNI调用Java中的非静态的方法
JNIEXPORT void JNICALL Java_JNIMain_accessMethod
(JNIEnv *env, jobject jobj) {
	//第一步找到jclass
	jclass jclz = (*env)->GetObjectClass(env, jobj);
	//第二步找到jMethodId  以上第四个参数是方法的签名
	jmethodID mid = (*env)->GetMethodID(env, jclz, "getRandomInt", "(I)I");
	//调用  
	jint random = (*env)->CallIntMethod(env, jobj, mid, 200);

	printf("C random :%d", random);
};
//JNI访问java中的静态方法  
JNIEXPORT void JNICALL Java_JNIMain_accessStaticMethod
(JNIEnv *env, jobject jobj) {
//jclass  首先通过jobject 搜索class  如果找到了将class  转变成jclass返回 
	jclass jclz=(*env)->GetObjectClass(env,jobj);
	//jmethodId     
	jmethodID mid = (*env)->GetStaticMethodID(env,jclz,"getRandeomUUId","()Ljava/lang/String;");


	//调用静态方法
	jstring uuid=(*env)->CallStaticObjectMethod(env, jclz, mid);
	  
	//将就jstring转化为char*
	char *uuid_c = (*env)->GetStringUTFChars(env,uuid,NULL);

	//uuid.txt
	char filename[100];
	sprintf(filename,"D://%s.txt",uuid_c);

	FILE *fp = fopen(filename,"w");
	fputs("I Love A", fp);
	fclose(fp);
	printf("\n文件写入成功");
};

//JNI访问Java中的构造方法
JNIEXPORT jobject JNICALL Java_JNIMain_accessConstructor
(JNIEnv *env, jobject jobj) {
	//为什么使用findClass    通过类的路径 从jvm中找到对应的类  
	jclass jclz = (*env)->FindClass(env, "java/util/Date");

	//jmethodId  地方参数表示的意思是  所有的构造方法的名字都是  <init>
	jmethodID mid = (*env)->GetMethodID(env, jclz, "<init>", "()V");

	//调用newObject 实例化Date对象,返回值是一个jobject
	jobject date_obj=(*env)->NewObject(env, jclz, mid);
	//使用方法必须找到ID
	jmethodID jmid=(*env)->GetMethodID(env, jclz, "getTime", "()J");

	jlong time=(*env)->CallLongMethod(env, date_obj, jmid);

	printf("time:%lld\n",time);
	return date_obj;

};
//JNIEXPORT jobject JNICALL Java_JNIMain_chineseChar
//(JNIEnv *env, jobject jobj,jstring in) {
//	jboolean iscp;
//	//第三个参数是用来得到这个值的 告诉我们  我们这个String是否是重新开辟的内存    这个方法会决定是否重新开辟块内存 
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
//	//jchar ->char    这里面有两个需要注意的地方  第一个就是申请足够大内存  第二个 必须对我们申请的内存进行初始化
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
//	//在java中会出现乱码的情况
//	printf("string:%s", rtn);
//
//	//只有当iscp是ture 的时候  我们才需要释放内存  
//	//(*env)->ReleaseStringUTFChars(env,in,c_str);
//	return NULL;
//};

JNIEXPORT jobject JNICALL Java_JNIMain_chineseChar
(JNIEnv *env, jobject jobj, jstring in) {
	char *c_str = "太阳和月亮";
	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	//对方法的签名  我们可以用javap命令生成
	jmethodID jmid = (*env)->GetMethodID(env, str_cls, "<init>", "([BLjava/lang/String;)V");

	//jstring ->jbyte
	jbyteArray bytes = (*env)->NewByteArray(env, strlen(c_str));

	//将char *  赋值到bytes   对c_str进行转换 转换成jbyteArray   

	(*env)->SetByteArrayRegion(env, bytes, 0, strlen(c_str), c_str);

	jstring chasetName = (*env)->NewStringUTF(env, "GB2312");

	return (*env)->NewObject(env, str_cls, jmid, bytes, chasetName);

};

//JNI中访问数组(基本类型数组)
int compare(jint *a, jint *b);

JNIEXPORT void JNICALL Java_JNIMain_giveArray
(JNIEnv *env, jobject jobj, jintArray arr) {
	//从jintArray -> jint *  jvm  会根据当前的情况  分配或者不分配一块内存 
	jint *elemets=(*env)->GetIntArrayElements(env, arr, NULL);
	//当内存不够的时候就为空
	if (elemets == NULL) {
		return;
	}
	//数组长度
	int len = (*env)->GetArrayLength(env, arr);

	qsort(elemets,len,sizeof(jint),compare);

	(*env)->ReleaseIntArrayElements(env, arr, elemets, JNI_COMMIT);

	//如果不写上面这一句 那么java中不会得到一个结果  上一句代码的作用是  1.释放有可能的缓存区域   
	//2.将我们jni的修改的数据从新写回原来的内存  所以当我们没有调用这个方法时候 ,我们得不到返回数据  

};
int compare(jint *a,jint *b){
	return *a - *b;

}

//访问引用类型的数组
JNIEXPORT jobjectArray JNICALL Java_JNIMain_initStringArray
(JNIEnv *env, jobject jobj, jint size) {
   //创建jobjectArray
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



		//赋值
	for (i = 0; i < size; i++) {
		char *c_str = (char*)malloc(256);
		memset(c_str, 0, 256);
		//将int转化成为char  
		sprintf(c_str, "hello num:%d\n", i);
	

	//c-->jstring
	jstring str = (*env)->NewStringUTF(env,c_str);

	if (str == NULL){
		return NULL;
	}

//	将jstring赋值给数组
//返回jobjectArray

	(*env)->SetObjectArrayElement(env, result, i, str);
	free(c_str);
	c_str = NULL;
	}

	return result;
};

//JNI引用

//局部引用    
//定义方式多样  :FindClass    NewObject   GetObjectClass  NewCharArray....  NewLocalFRef   用来在jni层创建在
//不能再多线程中使用 
JNIEXPORT jobjectArray JNICALL Java_JNIMain_localRef
(JNIEnv *env, jobject jobj) {
	int i = 0;
	for (i = 0; i < 5; i++) {
		jclass cls = (*env)->FindClass(env, "java/util/Date");
		jmethodID mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
		//创建了一个Date 类型的局部引用
		jobject obj = (*env)->NewObject(env, cls, mid);

		//使用这个引用

		//释放这个引用   如果没有调用这两个方法  那么jvm  会自动释放的   之所以需要手动释放的原因很简单 
		(*env)->DeleteLocalRef(env, cls);
		(*env)->DeleteLocalRef(env, obj);
	}

	return NULL;
};

//全局引用  
//跨前程 跨方法 
jstring global_str;
JNIEXPORT jobjectArray JNICALL Java_JNIMain_createGlobalRef
(JNIEnv *env, jobject jobj) {
	jobject obj = (*env)->NewStringUTF(env, "JNI is intersting");
	//这里是一个全局引用
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


//弱全局引用  
//他不会阻止GC  只要内存不够 就可以GC 
jclass  g_weak_cls;
JNIEXPORT jobjectArray JNICALL Java_JNIMain_createWeakRef
(JNIEnv *env, jobject jobj) {
	jclass cls_string = (*env)->FindClass(env, "java/lang/String");

	g_weak_cls = (*env)->NewWeakGlobalRef(env, cls_string);
	return g_weak_cls;


};


//JNI异常处理 

JNIEXPORT jstring JNICALL Java_JNIMain_exception
(JNIEnv *env, jobject jobj) {
	jclass cls = (*env)->GetObjectClass(env, jobj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "ksey", "Ljava/lang/String;");
	//检测是否发送异常
	jthrowable ex=(*env)->ExceptionOccurred(env);
	//判断时候有异常
	if (ex != NULL) {
		//清空JNI产生的异常
		(*env)->ExceptionClear(env);
		jclass newExc=(*env)->FindClass(env,"java/lang/ILl");

	}
	//上面的会出现异常,,这个还是会打印出来的 这个和java中是不一样的  java层也不能捕获JNI的异常  
	printf("exception");
};
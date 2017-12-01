
typedef struct JNINativeInterface_* JNIEnv;



struct JNINativeInterface_ {
	char* (*getString)(JNIEnv *, char);
};


char *getchar(JNIEnv* env, char c);

int main() {
	struct JNINativeInterface_ struct_JNI;
	//给函数指针赋值
	struct_JNI.getString = getchar;
	//上面做的就是初始化这个struct_JNI  结构体  


	JNIEnv en;//en是一个结构体指针  
	en = &struct_JNI;//为结构体指针赋值,取的地址是上面我们初始化的结构体


	JNIEnv *env = &en;
	(*env)->getString(env,"c");

	//总感觉调用了自己的方法   





}

typedef struct JNINativeInterface_* JNIEnv;



struct JNINativeInterface_ {
	char* (*getString)(JNIEnv *, char);
};


char *getchar(JNIEnv* env, char c);

int main() {
	struct JNINativeInterface_ struct_JNI;
	//������ָ�븳ֵ
	struct_JNI.getString = getchar;
	//�������ľ��ǳ�ʼ�����struct_JNI  �ṹ��  


	JNIEnv en;//en��һ���ṹ��ָ��  
	en = &struct_JNI;//Ϊ�ṹ��ָ�븳ֵ,ȡ�ĵ�ַ���������ǳ�ʼ���Ľṹ��


	JNIEnv *env = &en;
	(*env)->getString(env,"c");

	//�ܸо��������Լ��ķ���   





}
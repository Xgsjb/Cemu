#include "JNIUtils.h"

namespace JNIUtils
{
    JavaVM* g_jvm = nullptr;
};

std::string JNIUtils::JStringToString(JNIEnv* env, jstring jstr)
{
    const char* c_str = env->GetStringUTFChars(jstr, nullptr);
    std::string str(c_str);
    env->ReleaseStringUTFChars(jstr, c_str);
    return str;
}

jobject JNIUtils::createJavaStringArrayList(JNIEnv* env, const std::vector<std::string>& strings)
{
    jclass clsArrayList = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(clsArrayList, "<init>", "()V");
    jobject arrayListObject = env->NewObject(clsArrayList, arrayListConstructor);
    jmethodID addMethod = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

    for (const auto& string : strings)
    {
        jstring element = env->NewStringUTF(string.c_str());
        env->CallBooleanMethod(arrayListObject, addMethod, element);
        env->DeleteLocalRef(element);
    }

    env->DeleteLocalRef(clsArrayList);
    return arrayListObject;
}

jobject JNIUtils::createJavaLongArrayList(JNIEnv* env, const std::vector<uint64_t>& values)
{
    jclass longClass = env->FindClass("java/lang/Long");
    jmethodID valueOf = env->GetStaticMethodID(longClass, "valueOf", "(J)Ljava/lang/Long;");
    jobject arrayList = createArrayList(env, {});

    for (auto value : values)
    {
        jobject longObj = env->CallStaticObjectMethod(longClass, valueOf, static_cast<jlong>(value));
        env->CallBooleanMethod(arrayList, env->GetMethodID(longClass, "add", "(Ljava/lang/Object;)Z"), longObj);
        env->DeleteLocalRef(longObj);
    }

    env->DeleteLocalRef(longClass);
    return arrayList;
}

jobject JNIUtils::createJavaIntArrayList(JNIEnv* env, const std::vector<int>& values)
{
    jclass integerClass = env->FindClass("java/lang/Integer");
    jmethodID valueOf = env->GetStaticMethodID(integerClass, "valueOf", "(I)Ljava/lang/Integer;");
    jobject arrayList = createArrayList(env, {});

    for (int value : values)
    {
        jobject intObj = env->CallStaticObjectMethod(integerClass, valueOf, value);
        env->CallBooleanMethod(arrayList, env->GetMethodID(integerClass, "add", "(Ljava/lang/Object;)Z"), intObj);
        env->DeleteLocalRef(intObj);
    }

    env->DeleteLocalRef(integerClass);
    return arrayList;
}

jobject JNIUtils::createArrayList(JNIEnv* env, const std::vector<jobject>& objects)
{
    jclass listClass = env->FindClass("java/util/ArrayList");
    jmethodID listConstructor = env->GetMethodID(listClass, "<init>", "()V");
    jobject arrayList = env->NewObject(listClass, listConstructor);
    jmethodID listAdd = env->GetMethodID(listClass, "add", "(Ljava/lang/Object;)Z");

    for (auto&& obj : objects)
    {
        env->CallBooleanMethod(arrayList, listAdd, obj);
    }

    env->DeleteLocalRef(listClass);
    return arrayList;
}

JNIUtils::Scopedjobject JNIUtils::getEnumValue(JNIEnv* env, const std::string& enumClassName, const std::string& enumName)
{
    jclass enumClass = env->FindClass(enumClassName.c_str());
    jfieldID fieldID = env->GetStaticFieldID(enumClass, enumName.c_str(), ("L" + enumClassName + ";").c_str());
    jobject enumValue = env->GetStaticObjectField(enumClass, fieldID);
    env->DeleteLocalRef(enumClass);
    return Scopedjobject(enumValue);
}

jobject JNIUtils::createJavaStringArrayList(JNIEnv* env, const std::vector<std::wstring>& strings)
{
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
    jmethodID addMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    for (const auto& string : strings)
    {
        jstring element = env->NewString((const jchar*)string.c_str(), string.length());
        env->CallBooleanMethod(arrayListObject, addMethod, element);
        env->DeleteLocalRef(element);
    }

    env->DeleteLocalRef(arrayListClass);
    return arrayListObject;
}

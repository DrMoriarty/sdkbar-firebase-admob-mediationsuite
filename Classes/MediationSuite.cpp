#include "Firebase.hpp"
#include "MediationSuite.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/js_manual_conversions.h"
#include <sstream>
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include "utils/PluginUtils.h"
#include "firebase/admob.h"

extern std::string ApplicationId;

static void printLog(const char* str) {
    CCLOG("%s", str);
}

///////////////////////////////////////
//
//  Plugin Init
//
///////////////////////////////////////

static bool jsb_mediationsuite_launch_test_suite(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_mediationsuite_launch_test_suite");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 0) {
        if(ApplicationId.size() > 0) {

            cocos2d::JniMethodInfo methodInfo;
            if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "com/google/android/ads/mediationtestsuite/MediationTestSuite", "launch", "(Landroid/content/Context;Ljava/lang/String;)V")) {
                rec.rval().set(JSVAL_FALSE);
                return false;
            }
            jstring str = methodInfo.env->NewStringUTF(ApplicationId.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, cocos2d::JniHelper::getActivity(), str);
            methodInfo.env->DeleteLocalRef(str);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            rec.rval().set(JSVAL_TRUE);
        } else {
            rec.rval().set(JSVAL_FALSE);
        }
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}

///////////////////////////////////////
//
//  Register JS API
//
///////////////////////////////////////

void register_all_mediationsuite_framework(JSContext* cx, JS::HandleObject obj) {
    printLog("[AdMobMediationSuite] register js interface");
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "mediationsuite", &ns);

    JS_DefineFunction(cx, ns, "launch_test_suite", jsb_admob_launch_test_suite, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);
}

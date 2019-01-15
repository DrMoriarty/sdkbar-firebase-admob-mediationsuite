#import <Foundation/Foundation.h>
#import "MediationSuite.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/js_manual_conversions.h"
#include <sstream>
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include "utils/PluginUtils.h"
#include "firebase/app.h"
#include "firebase/admob.h"

#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#import <StoreKit/StoreKit.h>
#import <GoogleMobileAdsMediationTestSuite/GoogleMobileAdsMediationTestSuite.h>

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

            NSString *appID = [NSString stringWithUTF8String:ApplicationId.c_str()];
            UIViewController *vc = [UIApplication.sharedApplication.keyWindow rootViewController];
            [GoogleMobileAdsMediationTestSuite presentWithAppID:appID onViewController:vc delegate:nil];

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
    printLog("register_all_mediationsuite_framework");
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "mediationsuite", &ns);

    JS_DefineFunction(cx, ns, "launch_test_suite", jsb_mediationsuite_launch_test_suite, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);
}

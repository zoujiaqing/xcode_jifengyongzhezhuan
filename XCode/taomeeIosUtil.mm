#import "iosUtil.h"
#import "AppController.h"
#import "TaomeeFeedback/TaomeeFeedback.h"


TaomeeFeedback *feedback = NULL;
void onFeedbackViewOpen(int funid, int uid, int serverid, int channelid, const char *name){
    if(feedback == NULL){
    feedback = [TaomeeFeedback shared];
    feedback.userId = uid;
    feedback.forumId = @"79";
        feedback.channelId=[NSString stringWithFormat:@"%d",channelid];
        feedback.serverId = [NSString stringWithFormat:@"%d", serverid];
        feedback.userName = [NSString stringWithCString:name encoding:NSUTF8StringEncoding];
    feedback.viewController = (UIViewController *)[AppController rootController];
    feedback.delegate =(id)[AppController rootController];
    }
    switch (funid) {
        case 0:
            
            break;
        case 1:
        {//hot spot
            [feedback showFeedbackWithModule:TaomeeFeedbackFAQModule];
            break;
        }
        case 2:
            //forum
            [feedback showFeedbackWithModule:TaomeeFeedbackBBSModule];
            
            break;
        case 3:
        {
            [feedback showFeedbackWithModule:TaomeeFeedbackAnswerModule];
            break;
        }
        case 4:
        {
            [feedback showFeedbackWithModule:TaomeeFeedbackSubmitModule];
            break;
        }
        default:
            break;
    }
    
}
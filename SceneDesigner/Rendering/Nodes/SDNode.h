//
//  SDNode.h
//  SceneDesigner
//

#import "cocos2d.h"

extern NSString *CCNodeDidReorderChildren;
extern NSString *SDNodeUTI;

@protocol SDNodeProtocol <NSObject, NSCoding, NSPasteboardReading, NSPasteboardWriting>

@required

- (id)initWithDictionaryRepresentation:(NSDictionary *)dict;
- (NSDictionary *)dictionaryRepresentation;
- (NSArray *)snapPoints;

@optional

@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) BOOL isSelected;
@property (nonatomic, readwrite) CGFloat posX;
@property (nonatomic, readwrite) CGFloat posY;
@property (nonatomic, readwrite) CGFloat anchorX;
@property (nonatomic, readwrite) CGFloat anchorY;
@property (nonatomic, readwrite) CGFloat contentWidth;
@property (nonatomic, readwrite) CGFloat contentHeight;

- (NSDictionary *)_dictionaryRepresentation;
- (id)_initWithDictionaryRepresentation:(NSDictionary *)dict;

@end

#define SDNODE_IVARS \
NSString *_name;\
BOOL _isSelected;\
NSMutableArray *_snapPoints;\
BOOL _isDirtySnapPoints;

#define SDNODE_INIT() \
do\
{\
    [self setName:@"node"];\
} while (0)\

#define SDNODE_DEALLOC() \
do\
{\
    [_snapPoints release];\
    _snapPoints = nil;\
    [self setName:nil];\
} while (0)\

#define SDNODE_FUNC_SRC \
@synthesize name = _name;\
@synthesize isSelected = _isSelected;\
@dynamic posX;\
@dynamic posY;\
@dynamic anchorX;\
@dynamic anchorY;\
@dynamic contentWidth;\
@dynamic contentHeight;\
\
- (void)setPosition:(CGPoint)pos\
{\
    if ( [[[NSUserDefaults standardUserDefaults] valueForKey:@"nonIntegerPositions"] isEqualTo:[NSNumber numberWithBool:NO]] )\
    {\
        pos.x = floorf(pos.x);\
        pos.y = floorf(pos.y);\
    }\
\
    if (!CGPointEqualToPoint([self position], pos))\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setPosition:[self position]];\
        [um setActionName:NSLocalizedString(@"repositioning", nil)];\
\
        [self willChangeValueForKey:@"posX"];\
        [self willChangeValueForKey:@"posY"];\
        [super setPosition:pos];\
        [self didChangeValueForKey:@"posX"];\
        [self didChangeValueForKey:@"posY"];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
- (void)setAnchorPoint:(CGPoint)anchorPoint\
{\
    if (!CGPointEqualToPoint([self anchorPoint], anchorPoint))\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setAnchorPoint:[self anchorPoint]];\
        [um setActionName:NSLocalizedString(@"anchor point adjustment", nil)];\
\
        [self willChangeValueForKey:@"anchorX"];\
        [self willChangeValueForKey:@"anchorY"];\
        [super setAnchorPoint:anchorPoint];\
        [self didChangeValueForKey:@"anchorX"];\
        [self didChangeValueForKey:@"anchorY"];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setContentSize:(CGSize)size\
{\
    /* don't bother with undo for this,\
      only do undo for setContentWidth: or setContentHeight: */\
    if (!CGSizeEqualToSize([self contentSize], size))\
    {\
        [self willChangeValueForKey:@"contentWidth"];\
        [self willChangeValueForKey:@"contentHeight"];\
        [super setContentSize:size];\
        [self didChangeValueForKey:@"contentWidth"];\
        [self didChangeValueForKey:@"contentHeight"];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setScaleX:(float)sx\
{\
    if ([self scaleX] != sx)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setScaleX:[self scaleX]];\
        [um setActionName:NSLocalizedString(@"resizing", nil)];\
        [super setScaleX:sx];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setScaleY:(float)sy\
{\
    if ([self scaleY] != sy)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setScaleY:[self scaleY]];\
        [um setActionName:NSLocalizedString(@"resizing", nil)];\
        [super setScaleY:sy];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setZOrder:(NSInteger)z\
{\
    if ([self zOrder] != z)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setZOrder:[self zOrder]];\
        [um setActionName:NSLocalizedString(@"z order adjustment", nil)];\
        [super setZOrder:z];\
    }\
}\
\
- (void)setRotation:(float)rotation\
{\
    if ([self rotation] != rotation)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setRotation:[self rotation]];\
        [um setActionName:NSLocalizedString(@"rotation", nil)];\
        [super setRotation:rotation];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setTag:(NSInteger)tag\
{\
    if ([self tag] != tag)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setTag:[self tag]];\
        [um setActionName:NSLocalizedString(@"tag adjustment", nil)];\
        [super setTag:tag];\
    }\
}\
\
- (void)setVisible:(BOOL)visible\
{\
    if ([self visible] != visible)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setVisible:[self visible]];\
        [um setActionName:NSLocalizedString(@"visibility adjustment", nil)];\
        [super setVisible:visible];\
\
        _isDirtySnapPoints = YES;\
    }\
}\
\
- (void)setIgnoreAnchorPointForPosition:(BOOL)relative\
{\
    if ([self ignoreAnchorPointForPosition] != relative)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setIgnoreAnchorPointForPosition:[self ignoreAnchorPointForPosition]];\
        [um setActionName:NSLocalizedString(@"ignore anchor point adjustment", nil)];\
        [super setIgnoreAnchorPointForPosition:relative];\
    }\
}\
\
- (void)setName:(NSString *)name\
{\
    if (![name isEqualToString:[self name]])\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [(CCNode<SDNodeProtocol> *)[um prepareWithInvocationTarget:self] setName:[self name]];\
        [um setActionName:NSLocalizedString(@"renaming", nil)];\
        [_name release];\
        _name = [[[SDUtils sharedUtils] uniqueNameForString:name] copy];\
    }\
}\
\
- (CGFloat)posX\
{\
    return self.position.x;\
}\
\
- (void)setPosX:(CGFloat)posX\
{\
    if (posX != self.position.x)\
    {\
        CGPoint pos = self.position;\
        pos.x = posX;\
        self.position = pos;\
    }\
}\
\
- (CGFloat)posY\
{\
    return self.position.y;\
}\
\
- (void)setPosY:(CGFloat)posY\
{\
    if (posY != self.position.y)\
    {\
        CGPoint pos = self.position;\
        pos.y = posY;\
        self.position = pos;\
    }\
}\
\
- (CGFloat)anchorX\
{\
    return self.anchorPoint.x;\
}\
\
- (void)setAnchorX:(CGFloat)anchorX\
{\
    if (anchorX != self.anchorPoint.x)\
    {\
        CGPoint anchor = self.anchorPoint;\
        anchor.x = anchorX;\
        self.anchorPoint = anchor;\
    }\
}\
\
- (CGFloat)anchorY\
{\
    return self.anchorPoint.y;\
}\
\
- (void)setAnchorY:(CGFloat)anchorY\
{\
    if (anchorY != self.position.y)\
    {\
        CGPoint anchor = self.anchorPoint;\
        anchor.y = anchorY;\
        self.anchorPoint = anchor;\
    }\
}\
\
- (CGFloat)contentWidth\
{\
    return self.contentSize.width;\
}\
\
- (void)setContentWidth:(CGFloat)contentWidth\
{\
    if (contentWidth != self.contentSize.width)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setContentWidth:self.contentSize.width];\
        [um setActionName:NSLocalizedString(@"content size adjustment", nil)];\
\
        CGSize size = self.contentSize;\
        size.width = contentWidth;\
        self.contentSize = size;\
    }\
}\
\
- (CGFloat)contentHeight\
{\
    return self.contentSize.height;\
}\
\
- (void)setContentHeight:(CGFloat)contentHeight\
{\
    if (contentHeight != self.contentSize.height)\
    {\
        NSUndoManager *um = [[SDUtils sharedUtils] currentUndoManager];\
        [[um prepareWithInvocationTarget:self] setContentHeight:self.contentSize.height];\
        [um setActionName:NSLocalizedString(@"content size adjustment", nil)];\
\
        CGSize size = self.contentSize;\
        size.height = contentHeight;\
        self.contentSize = size;\
    }\
}\
\
- (void)sortAllChildren\
{\
    BOOL shouldPostNotification = isReorderChildDirty_;\
    [super sortAllChildren];\
    if (shouldPostNotification)\
        [[NSNotificationCenter defaultCenter] postNotificationName:CCNodeDidReorderChildren object:self];\
}\
\
- (NSArray *)snapPoints\
{\
    if (floorf([self rotation]) != [self rotation] || (int)floorf([self rotation]) % 360 != 0 || ![self visible])\
        return [NSArray array];\
\
    if (_snapPoints == nil)\
    {\
        _isDirtySnapPoints = YES;\
        _snapPoints = [[NSMutableArray arrayWithCapacity:5] retain];\
    }\
\
    if (_isDirtySnapPoints)\
    {\
        CGSize s = [self contentSize];\
        CGPoint p1 = [self convertToWorldSpace:ccp(0,0)]; /*bl*/\
        CGPoint p2 = [self convertToWorldSpace:ccp(s.width,0)]; /*br*/\
        CGPoint p3 = [self convertToWorldSpace:ccp(s.width,s.height)]; /*tr*/\
        CGPoint p4 = [self convertToWorldSpace:ccp(0,s.height)]; /*tl*/\
        /*CGPoint p5 = [self convertToWorldSpaceAR:ccp(0,0)];*/ /*anchor point*/\
\
        [_snapPoints removeAllObjects];\
        [_snapPoints addObject:[NSValue value:&p1 withObjCType:@encode(CGPoint)]];\
        [_snapPoints addObject:[NSValue value:&p2 withObjCType:@encode(CGPoint)]];\
        [_snapPoints addObject:[NSValue value:&p3 withObjCType:@encode(CGPoint)]];\
        [_snapPoints addObject:[NSValue value:&p4 withObjCType:@encode(CGPoint)]];\
        /*[_snapPoints addObject:[NSValue value:&p5 withObjCType:@encode(CGPoint)]];*/\
    }\
\
    return [NSArray arrayWithArray:_snapPoints];\
}\
\
- (NSDictionary *)dictionaryRepresentation\
{\
    NSMutableDictionary *dict = nil;\
    if ([self respondsToSelector:@selector(_dictionaryRepresentation)])\
        dict = [[[self _dictionaryRepresentation] mutableCopy] autorelease];\
    else\
        dict = [NSMutableDictionary dictionaryWithCapacity:11];\
\
    [dict setObject:NSStringFromClass([[SDUtils sharedUtils] cocosClassFromCustomClass:[self class]]) forKey:@"className"];\
    [dict setObject:((self.name != nil) ? self.name : @"") forKey:@"name"];\
    [dict setObject:NSStringFromPoint(NSPointFromCGPoint(self.position)) forKey:@"position"];\
    [dict setObject:NSStringFromPoint(NSPointFromCGPoint(self.anchorPoint)) forKey:@"anchorPoint"];\
    [dict setObject:[NSNumber numberWithFloat:self.scaleX] forKey:@"scaleX"];\
    [dict setObject:[NSNumber numberWithFloat:self.scaleY] forKey:@"scaleY"];\
    [dict setObject:NSStringFromSize(NSSizeFromCGSize(self.contentSize)) forKey:@"contentSize"];\
    [dict setObject:[NSNumber numberWithInteger:self.zOrder] forKey:@"zOrder"];\
    [dict setObject:[NSNumber numberWithFloat:self.rotation] forKey:@"rotation"];\
    [dict setObject:[NSNumber numberWithInteger:self.tag] forKey:@"tag"];\
    [dict setObject:[NSNumber numberWithBool:self.visible] forKey:@"visible"];\
    [dict setObject:[NSNumber numberWithBool:self.ignoreAnchorPointForPosition] forKey:@"ignoreAnchorPointForPosition"];\
\
    NSMutableArray *children = [NSMutableArray array];\
    if ([[self children] count] > 0)\
        for (CCNode<SDNodeProtocol> *child in [self children])\
            if ([child isKindOfClass:[CCNode class]] && [child conformsToProtocol:@protocol(SDNodeProtocol)])\
                [children addObject:[child dictionaryRepresentation]];\
\
    [dict setObject:children forKey:@"children"];\
\
    return dict;\
}\
- (id)initWithDictionaryRepresentation:(NSDictionary *)dict\
{\
    if ([self respondsToSelector:@selector(_initWithDictionaryRepresentation:)])\
        self = [self _initWithDictionaryRepresentation:dict];\
    else\
        self = [self init];\
\
    if (self)\
    {\
        self.name = [dict valueForKey:@"name"];\
        self.position = NSPointToCGPoint(NSPointFromString([dict valueForKey:@"position"]));\
        self.anchorPoint = NSPointToCGPoint(NSPointFromString([dict valueForKey:@"anchorPoint"]));\
        self.scaleX = [[dict valueForKey:@"scaleX"] floatValue];\
        self.scaleY = [[dict valueForKey:@"scaleY"] floatValue];\
        self.contentSize = NSSizeToCGSize(NSSizeFromString([dict valueForKey:@"contentSize"]));\
        self.rotation = [[dict valueForKey:@"rotation"] floatValue];\
        self.tag = [[dict valueForKey:@"tag"] integerValue];\
        self.visible = [[dict valueForKey:@"visible"] boolValue];\
        self.ignoreAnchorPointForPosition = [[dict valueForKey:@"ignoreAnchorPointForPosition"] boolValue];\
        self.zOrder = [[dict valueForKey:@"zOrder"] integerValue];\
\
        NSArray *children = [dict objectForKey:@"children"];\
        for (NSDictionary *child in children)\
        {\
            Class childClass = [[SDUtils sharedUtils] customClassFromCocosClass:NSClassFromString([child valueForKey:@"className"])];\
            if (childClass && [childClass isSubclassOfClass:[CCNode class]] && [childClass conformsToProtocol:@protocol(SDNodeProtocol)])\
            {\
                CCNode<SDNodeProtocol> *node = [[[childClass alloc] initWithDictionaryRepresentation:child] autorelease];\
                [self addChild:node];\
            }\
        }\
    }\
\
    return self;\
}\
\
- (id)initWithCoder:(NSCoder *)aDecoder\
{\
    NSDictionary *dict = [aDecoder decodeObjectForKey:@"dictionaryRepresentation"];\
    if (!dict)\
        return nil;\
\
    self = [self initWithDictionaryRepresentation:dict];\
    return self;\
}\
\
- (void)encodeWithCoder:(NSCoder *)aCoder\
{\
    [aCoder encodeObject:[self dictionaryRepresentation] forKey:@"dictionaryRepresentation"];\
}\
\
- (NSArray *)writableTypesForPasteboard:(NSPasteboard *)pasteboard\
{\
    return [NSArray arrayWithObject:SDNodeUTI];\
}\
\
- (id)pasteboardPropertyListForType:(NSString *)type\
{\
    if ([type isEqualToString:SDNodeUTI])\
        return [NSKeyedArchiver archivedDataWithRootObject:self];\
    \
    return nil;\
}\
\
+ (NSArray *)readableTypesForPasteboard:(NSPasteboard *)pasteboard\
{\
    return [NSArray arrayWithObject:SDNodeUTI];\
}\
\
+ (NSPasteboardReadingOptions)readingOptionsForType:(NSString *)type pasteboard:(NSPasteboard *)pasteboard\
{\
    if ([type isEqualToString:SDNodeUTI])\
        return NSPasteboardReadingAsKeyedArchive;\
\
    return 0;\
}\
\
- (void)setValue:(id)value forUndefinedKey:(NSString *)key\
{\
    /* dont do anything */\
}\
\
- (id)valueForUndefinedKey:(NSString *)key\
{\
    return nil;\
}\

/**
 * This is the basic SDNode without any additional modifications
 */
@interface SDNode : CCNode <SDNodeProtocol>
{
    SDNODE_IVARS
}

@end

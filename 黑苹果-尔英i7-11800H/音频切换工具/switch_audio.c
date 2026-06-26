#include <AudioToolbox/AudioToolbox.h>
#include <stdio.h>
#include <string.h>

void list_devices() {
    AudioObjectPropertyAddress addr = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    UInt32 size = 0;
    AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &addr, 0, NULL, &size);
    int count = size / sizeof(AudioDeviceID);
    AudioDeviceID devs[count];
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr, 0, NULL, &size, devs);
    
    AudioDeviceID defIn, defOut;
    size = sizeof(defIn);
    addr.mSelector = kAudioHardwarePropertyDefaultInputDevice;
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr, 0, NULL, &size, &defIn);
    addr.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr, 0, NULL, &size, &defOut);
    
    for (int i = 0; i < count; i++) {
        CFStringRef name = NULL;
        size = sizeof(name);
        AudioObjectPropertyAddress naddr = {
            kAudioDevicePropertyDeviceNameCFString,
            kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain
        };
        AudioObjectGetPropertyData(devs[i], &naddr, 0, NULL, &size, &name);
        if (!name) continue;
        char buf[256];
        CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8);
        
        AudioObjectPropertyAddress iaddr = {
            kAudioDevicePropertyStreams,
            kAudioDevicePropertyScopeInput,
            kAudioObjectPropertyElementMain
        };
        UInt32 isize = 0;
        if (AudioObjectGetPropertyDataSize(devs[i], &iaddr, 0, NULL, &isize) == noErr)
            printf("🎤 IN:  %s%s\n", buf, devs[i] == defIn ? " [默认]" : "");
        
        AudioObjectPropertyAddress oaddr = {
            kAudioDevicePropertyStreams,
            kAudioDevicePropertyScopeOutput,
            kAudioObjectPropertyElementMain
        };
        UInt32 osize = 0;
        if (AudioObjectGetPropertyDataSize(devs[i], &oaddr, 0, NULL, &osize) == noErr)
            printf("🔊 OUT: %s%s\n", buf, devs[i] == defOut ? " [默认]" : "");
        
        CFRelease(name);
    }
}

int set_device(const char *target, int isOutput) {
    AudioObjectPropertyAddress addr = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    UInt32 size = 0;
    AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &addr, 0, NULL, &size);
    int count = size / sizeof(AudioDeviceID);
    AudioDeviceID devs[count];
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr, 0, NULL, &size, devs);
    
    for (int i = 0; i < count; i++) {
        CFStringRef name = NULL;
        size = sizeof(name);
        AudioObjectPropertyAddress naddr = {
            kAudioDevicePropertyDeviceNameCFString,
            kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain
        };
        AudioObjectGetPropertyData(devs[i], &naddr, 0, NULL, &size, &name);
        if (!name) continue;
        char buf[256];
        CFStringGetCString(name, buf, sizeof(buf), kCFStringEncodingUTF8);
        
        if (strstr(buf, target)) {
            AudioObjectPropertyAddress saddr = {
                isOutput ? kAudioHardwarePropertyDefaultOutputDevice : kAudioHardwarePropertyDefaultInputDevice,
                kAudioObjectPropertyScopeGlobal,
                kAudioObjectPropertyElementMain
            };
            OSStatus err = AudioObjectSetPropertyData(
                kAudioObjectSystemObject, &saddr, 0, NULL,
                sizeof(AudioDeviceID), &devs[i]
            );
            CFRelease(name);
            if (err == noErr) {
                printf("✅ %s → %s\n", isOutput ? "输出" : "输入", buf);
                return 0;
            }
            printf("❌ 失败: %d\n", err);
            return 1;
        }
        CFRelease(name);
    }
    printf("❌ 找不到设备: %s\n", target);
    return 1;
}

int main(int argc, char **argv) {
    if (argc == 1) { list_devices(); return 0; }
    if (argc >= 2 && strcmp(argv[1], "in") == 0) return set_device(argv[2], 0);
    if (argc >= 2 && strcmp(argv[1], "out") == 0) return set_device(argv[2], 1);
    printf("用法: %s [in|out] <设备名>\n", argv[0]);
    return 1;
}

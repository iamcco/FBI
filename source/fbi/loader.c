// reference:
// https://github.com/devkitPro/3ds-hbmenu/blob/master/source/loaders/rosalina.c

#include "loader.h"

static Handle hbldrHandle;

static Result init(void) { return svcConnectToPort(&hbldrHandle, "hb:ldr"); }

static Result HBLDR_SetTarget(const char *path) {
  u32 pathLen = strlen(path) + 1;
  u32 *cmdbuf = getThreadCommandBuffer();

  cmdbuf[0] = IPC_MakeHeader(2, 0, 2); // 0x20002
  cmdbuf[1] = IPC_Desc_StaticBuffer(pathLen, 0);
  cmdbuf[2] = (u32)path;

  Result rc = svcSendSyncRequest(hbldrHandle);
  if (R_SUCCEEDED(rc))
    rc = cmdbuf[1];
  return rc;
}

static Result HBLDR_SetArgv(const void *buffer, u32 size) {
  u32 *cmdbuf = getThreadCommandBuffer();

  cmdbuf[0] = IPC_MakeHeader(3, 0, 2); // 0x30002
  cmdbuf[1] = IPC_Desc_StaticBuffer(size, 1);
  cmdbuf[2] = (u32)buffer;

  Result rc = svcSendSyncRequest(hbldrHandle);
  if (R_SUCCEEDED(rc))
    rc = cmdbuf[1];
  return rc;
}

static void deinit(void) { svcCloseHandle(hbldrHandle); }

Result loader_launch_file(const char *path, const char *input_args) {
  // init
  Result rc = init();
  if (R_SUCCEEDED(rc)) {
    // args string
    argData_s args;
    memset(args.buf, '\0', sizeof(args.buf));

    // append 3dsx path
    args.dst = (char *)&args.buf[1];
    strcpy(args.dst, path);
    args.dst += strlen(path) + 1;
    args.buf[0]++;

    // append args
    if (input_args != NULL) {
      strcpy(args.dst, input_args);
      args.dst += strlen(input_args) + 1;
      args.buf[0]++;
    }

    // path fix
    if (strncmp(path, "sdmc:/", 6) == 0) {
      path += 5;
    }

    // set launch path
    HBLDR_SetTarget(path);
    // set launch args
    HBLDR_SetArgv(args.buf, sizeof(args.buf));
    // deinit
    deinit();
  }

  return rc;
}

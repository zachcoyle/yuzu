// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <memory>
#include "common/common_types.h"
#include "common/logging/log.h"
#include "core/file_sys/nca_metadata.h"
#include "core/file_sys/registered_cache.h"
#include "core/file_sys/romfs_factory.h"
#include "core/hle/kernel/process.h"
#include "core/hle/service/filesystem/filesystem.h"
#include "core/loader/loader.h"

namespace FileSys {

RomFSFactory::RomFSFactory(Loader::AppLoader& app_loader) {
    // Load the RomFS from the app
    if (app_loader.ReadRomFS(file) != Loader::ResultStatus::Success) {
        LOG_ERROR(Service_FS, "Unable to read RomFS!");
    }
}

ResultVal<VirtualFile> RomFSFactory::OpenCurrentProcess() {
    return MakeResult<VirtualFile>(file);
}

ResultVal<VirtualFile> RomFSFactory::Open(u64 title_id, StorageId storage, ContentRecordType type) {
    switch (storage) {
    case StorageId::NandSystem: {
        const auto res = Service::FileSystem::GetSystemNANDContents()->GetEntry(title_id, type);
        if (res == nullptr) {
            // TODO(DarkLordZach): Find the right error code to use here
            return ResultCode(-1);
        }
        const auto romfs = res->GetRomFS();
        if (romfs == nullptr) {
            // TODO(DarkLordZach): Find the right error code to use here
            return ResultCode(-1);
        }
        return MakeResult<VirtualFile>(romfs);
    }
    case StorageId::NandUser: {
        const auto res = Service::FileSystem::GetUserNANDContents()->GetEntry(title_id, type);
        if (res == nullptr) {
            // TODO(DarkLordZach): Find the right error code to use here
            return ResultCode(-1);
        }
        const auto romfs = res->GetRomFS();
        if (romfs == nullptr) {
            // TODO(DarkLordZach): Find the right error code to use here
            return ResultCode(-1);
        }
        return MakeResult<VirtualFile>(romfs);
    }
    default:
        UNIMPLEMENTED_MSG("Unimplemented storage_id={:02X}", static_cast<u8>(storage));
    }
}

} // namespace FileSys

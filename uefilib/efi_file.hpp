#pragma once
#include "efi_types.hpp"
#include "efi_GUID.hpp"
namespace efi
{
    enum class FileMode : uint64_t
    {
        READ = 0x0000000000000001,
        WRITE = 0x0000000000000002,
        CREATE = 0x8000000000000000
    };

    enum class FileAttributes : uint64_t
    {
        FILE_READ_ONLY = 0x0000000000000001,
        FILE_HIDDEN = 0x0000000000000002,
        FILE_SYSTEM = 0x0000000000000004,
        FILE_RESERVED = 0x0000000000000008,
        FILE_DIRECTORY = 0x0000000000000010,
        FILE_ARCHIVE = 0x0000000000000020,
        FILE_VALID_ATTR = 0x0000000000000037
    };

    struct FileIOToken
    {
        Event event;
        Status status;
        uint64_t bufferSize;
        void* buffer;
    };

    struct FileProtocol
    {
        uint64_t revision;
        Status (*open)(FileProtocol* file, FileProtocol** newHandle, const char16_t* fileName, FileMode openMode, FileAttributes Attributes);
        Status (*close)(FileProtocol* file);
        Status (*deletefile)(FileProtocol* file);
        Status (*read)(FileProtocol* file, uint64_t* bufferSize, void* buffer);
        Status (*write)(FileProtocol* file, uint64_t* bufferSize, void* buffer);
        Status (*getPosition)(FileProtocol* file, uint64_t* position);
        Status (*setPosition)(FileProtocol* file, uint64_t position);
        Status (*getInfo)(FileProtocol* file, const GUID* informationType, uint64_t* bufferSize, void* buffer);
        Status (*setInfo)(FileProtocol* file, const GUID* informationType, uint64_t bufferSize, void* buffer);
        Status (*flush)(FileProtocol* file);
        Status (*openEx)(FileProtocol* file, FileProtocol** newHandle, const char16_t* fileName, FileMode openMode, FileAttributes attributes, FileIOToken* token);
        Status(*readEx)(FileProtocol* file, FileIOToken* token);
        Status (*writeEx)(FileProtocol* file, FileIOToken* token);
        Status (*flushEx)(FileProtocol* file, FileIOToken* token);
    };
    using FileHandle = FileProtocol*;
    using File = FileProtocol;
    struct FileInfo
    {
        uint64_t size;
        uint64_t fileSize;
        uint64_t physicalSize;
        Time createTime;
        Time lastAccessTime;
        Time modificationTime;
        FileAttributes attributes;
        char16_t fileName[1];
    };

    struct FileSystemInfo
    {
        uint64_t size;
        bool readOnly;
        uint64_t volumeSize;
        uint64_t freeSpace;
        uint32_t blockSize;
        char16_t volumeLabel[1];
    };

    struct LoadedImageProtocol
    {
        uint32_t revision;
        Handle parentHandle;
        SystemTable* SystemTable;

        Handle deviceHandle;
        DevicePath* filePath;
        void* Reserved;

        uint32_t loadOptionsSize;
        void* loadOptions;

        void* imageBase;
        uint64_t imageSize;
        MemoryType imageCodeType;
        MemoryType imageDataType;

        Status (*unload) (Handle imageHandle);
    };

    struct SimpleFileSystemProtocol
    {
        uint64_t revision;
        Status (*openVolume) (SimpleFileSystemProtocol* simpleFileSystemProtocol, FileProtocol** root);
    };
}
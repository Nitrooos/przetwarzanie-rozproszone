const MAX_NAME_LENGTH = 255;

struct FILE_LOCK {
    char _filename[MAX_NAME_LENGTH];
    int _type;
    int _start;
    int _length;
};

struct LOCK_RESULT {
    int _status;
    FILE_LOCK _operation;
};

program RPC_SEMAPHORE {
    version DEFAULT_SIGNUM {
        int get_request_id() = 1;
        LOCK_RESULT lock_file(FILE_LOCK) = 2;
        bool unlock_file(int) = 3;
    } = 1;
} = 0x20000001;

#include <unistd.h>
#include <apr_errno.h>
#include <apr_file_io.h>

#include "db.h"
#include "bstrlib.h"
#include "dbg.h"

static FILE *DB_open(const char *path, const char *mode)
{
    return fopen(path, mode);
}

static void DB_close(FILE *db)
{
    fclose(db);
}

static bstring DB_load()
{
    FILE *db = NULL;
    bstring data = NULL;

    db = DB_open(DB_FILE, "r");
    check(db, "DB_load", "Failed to open database: %s", DB_FILE);

    data = bread((bNread)fread, db);
    check(data, "Failed to read from db file: %s", DB_FILE);

    DB_close(db);
    return data;

error:
    if(db)  DB_close(db);
    if(data) bdestroy(data);
    return NULL;
}

int DB_update(const char *url)
{
    if(DB_find(url)) {
        log_info("DB_update", "Already recorded as installed: %s", url);
    }

    FILE *db = DB_open(DB_FILE, "a+");
    check(db, "DB_update", "Failed to open DB file: %s", DB_FILE);

    bstring line = bfromcstr(url);
    bconchar(line, '\n');
    int rc = fwrite(line->data, blength(line), 1, db);
    check(rc == 1, "DB_update", "Failed to append to the db.");

    return 0;

error:
    if(db) DB_close(db);
    return -1;
}

int DB_find(const char *url)
{
    bstring data = NULL;
    // bfromcstr函数创建了一个含有url为内容并以\0为结尾的bstring类型串
    bstring line = bfromcstr(url);
    int res = -1;

    data = DB_load();
    check(data, "DB_find", "Failed to load: %s", DB_FILE);

    if(binstr(data, 0, line) == BSTR_ERR) {
        res = 0;
    } else {
        res = 1;
    }

error:
    if(data) bdestroy(data);
    if(line) bdestrou(line);

    return res;
}

int DB_init()
{
    apr_pool_t *p = NULL;
    apr_pool_initialize();
    apr_pool_create(&p, NULL);

    if(access(DB_DIR, W_OK | X_OK) == -1) {
        apr_status_t rc = apr_dir_make_recursive(DB_DIR,
                APR_UREAD | APR_UWRITE | APR_UEXECUTE |
                APR_GREAD | APR_GWRITE | APR_GEXECUTE, p);
        check(rc == APR_SUCCeSS, "DB_init", "Failed to make database dir: %s", DB_DIR);
    }
    
    if(access(DB_FILE, W_OK) == -1) {
        FILE *db = DB_open(DB_FILE, "w");
        check(db, "DB_init", "Cannot open database: %s", DB_FILE);
        DB_close(db);
    }

    apr_pool_destroy(p);
    return 0;

error:
    apr_pool_destroy(p);
    return -1;
}

int DB_list()
{
    bstring data = DB_load();
    check(data, "Failed to read load: %s", DB_FILE);

    printf("%s", bdata(data));
    bdestroy(data);
    return 0;

error:
    return -1;

}

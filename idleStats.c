#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

// Buffer struct
typedef struct BufferStruct
{
    char *buffer;
    size_t size;
} BufferStruct;

// Function that curl calls to write to buffer
static size_t WriteMemoryCallback (void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct BufferStruct * mem = (struct BufferStruct *) data;
    mem->buffer = realloc(mem->buffer, mem->size + realsize + 1);
    if ( mem->buffer )
    {
        memcpy( &( mem->buffer[ mem->size ] ), ptr, realsize );
        mem->size += realsize;
        mem->buffer[ mem->size ] = 0;
    }
    return realsize;
}

int main(void)
{
    // Init everything and define varibles
    curl_global_init( CURL_GLOBAL_ALL );
    CURL *curl = curl_easy_init();
    if (!curl) 
    { printf("[ERROR] Failed to init curl.\n"); return 1; }
    CURLcode result;
    char url[200];
    char userId[50];
    char token[20] = "";
    char authHeader[35] = "";
    struct BufferStruct output;
    struct curl_slist *headers = NULL;
    output.buffer = NULL;
    output.size = 0;

    // Get needed info and build url and token header
    strcat(url, "https://public-api.travitia.xyz/idle/profile?user=");
    strcat(authHeader, "Authorization: ");
    printf("[INFO] Please enter your API token.\n");
    scanf("%s", token);
    printf("[INFO] Please enter the user id.\n");
    scanf("%s", userId);
    strcat(authHeader, token);
    strcat(url, userId);

    // Connect to the API and get the data
    printf("[INFO] Grabing data from %s\n[INFO] Using %s as token.\n", url, token);
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&output);
    headers = curl_slist_append(headers, authHeader);
    curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt( curl, CURLOPT_URL, url);
    result = curl_easy_perform( curl );

    // Check if the data is there
    if (result != CURLE_OK)
    {
        printf("[ERROR] Curl failed with error code %d\n", result);
        printf("[ERROR] %s\n",curl_easy_strerror(result));
    }
    curl_easy_cleanup( curl );

    // Save the data
    FILE *fp;
    fp = fopen("data.json", "w");
    if (!fp)
    { printf("[ERROR] Could not open data.json.\n"); return 1; }
    fprintf(fp, output.buffer);
    printf("[INFO] Successfully written data to data.json\n");
    fclose( fp );

    // define the varibles for parsing the json
    struct json_object *parsed_json;
    struct json_object *user;
    struct json_object *name;
    struct json_object *money;
    struct json_object *xp;
    struct json_object *pvpwins;
    struct json_object *moneyBooster;
    struct json_object *timeBooster;
    struct json_object *luckBooster;
    struct json_object *marriage;
    struct json_object *color;
    struct json_object *background;
    struct json_object *guild;
    struct json_object *classes;
    struct json_object *class;
    size_t n_classes;
    struct json_object *deaths;
    struct json_object *completed;
    struct json_object *lovescore;
    struct json_object *guildrank;
    struct json_object *puzzles;
    struct json_object *atkMultiply;
    struct json_object *defMultiply;
    struct json_object *trickOrTreat;
    struct json_object *eastereggs;
    struct json_object *cratesCommon;
    struct json_object *cratesUncommon;
    struct json_object *cratesRare;
    struct json_object *cratesMagic;
    struct json_object *cratesLegendary;
    struct json_object *luck;
    struct json_object *god;
    struct json_object *favor;
    struct json_object *race;
    struct json_object *cv;

    // Parse everything
    parsed_json = json_tokener_parse(output.buffer);
    if (!parsed_json)
    {
        printf("[ERROR] Failed to parse json.\n");
        return 1;
    }

    parsed_json = json_object_array_get_idx(parsed_json, 0);
    json_object_object_get_ex(parsed_json, "user", &user);
    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "money", &money);
    json_object_object_get_ex(parsed_json, "xp", &xp);
    json_object_object_get_ex(parsed_json, "pvpwins", &pvpwins);
    json_object_object_get_ex(parsed_json, "money_booster", &moneyBooster);
    json_object_object_get_ex(parsed_json, "time_booster", &timeBooster);
    json_object_object_get_ex(parsed_json, "luck_booster", &luckBooster);
    json_object_object_get_ex(parsed_json, "marriage", &marriage);
    json_object_object_get_ex(parsed_json, "colour", &color);
    json_object_object_get_ex(parsed_json, "backgroun", &background);
    json_object_object_get_ex(parsed_json, "guild", &guild);
    json_object_object_get_ex(parsed_json, "class", &classes);
    json_object_object_get_ex(parsed_json, "deaths", &deaths);
    json_object_object_get_ex(parsed_json, "completed", &completed);
    json_object_object_get_ex(parsed_json, "lovescore", &lovescore);
    json_object_object_get_ex(parsed_json, "guildrank", &guildrank);
    json_object_object_get_ex(parsed_json, "puzzles", &puzzles);
    json_object_object_get_ex(parsed_json, "atkmultiply", &atkMultiply);
    json_object_object_get_ex(parsed_json, "defmultiply", &defMultiply);
    json_object_object_get_ex(parsed_json, "trickortreat", &trickOrTreat);
    json_object_object_get_ex(parsed_json, "eastereggs", &eastereggs);
    json_object_object_get_ex(parsed_json, "crates_common", &cratesCommon);
    json_object_object_get_ex(parsed_json, "crates_uncommon", &cratesUncommon);
    json_object_object_get_ex(parsed_json, "crates_rare", &cratesRare);
    json_object_object_get_ex(parsed_json, "crates_magic", &cratesMagic);
    json_object_object_get_ex(parsed_json, "crates_legendary",&cratesLegendary);
    json_object_object_get_ex(parsed_json, "luck", &luck);
    json_object_object_get_ex(parsed_json, "god", &god);
    json_object_object_get_ex(parsed_json, "favor", &favor);
    json_object_object_get_ex(parsed_json, "race", &race);
    json_object_object_get_ex(parsed_json, "cv", &cv);

    // Print all the newly parsed info
    printf("User id: %d\n", json_object_get_int(user));
    printf("Name: %s\n", json_object_get_string(name));
    printf("Money: %d\n", json_object_get_int(money));
    printf("XP: %d\n", json_object_get_int(xp));
    printf("PVP Wins: %d\n", json_object_get_int(pvpwins));
    printf("Number of money boosters: %d\n", json_object_get_int(moneyBooster));
    printf("Number of time boosters: %d\n", json_object_get_int(timeBooster));
    printf("Number of luck boosters: %d\n", json_object_get_int(luckBooster));
    printf("User ID of spouse: %d\n", json_object_get_int(marriage));
    printf("Profile color: %s\n", json_object_get_string(color));
    printf("Url of background image: %s\n", json_object_get_string(background));
    printf("Guild ID number: %d\n", json_object_get_int(guild));
    n_classes = json_object_array_length(classes);
    printf("Number of classes: %lu\n", n_classes);
    for (size_t i = 0; i < n_classes; i++)
    {
        class = json_object_array_get_idx(classes, i);
        printf("    %lu. %s\n", i+1, json_object_get_string(class));
    }
    printf("Number of deaths: %d\n", json_object_get_int(deaths));
    printf("Total completed adventures: %d\n", json_object_get_int(completed));
    float adventureAverage = (json_object_get_int(completed) / json_object_get_int(deaths)) * 100;
    printf("Adventure success percentage: %1.0f %\n", adventureAverage);
    printf("Lovescore: %d\n", json_object_get_int(lovescore));
    printf("Current guild rank: %s\n", json_object_get_string(guildrank));
    printf("Completed puzzles: %d\n", json_object_get_int(puzzles));
    printf("Attack multiplier: %0.1f\n", json_object_get_double(atkMultiply));
    printf("Defense multiplier: %0.1f\n", json_object_get_double(defMultiply));
    printf("Trick or treat: %d\n", json_object_get_int(trickOrTreat));
    printf("Easter Eggs: %d\n", json_object_get_int(eastereggs));
    printf("Number of common crates: %d\n", json_object_get_int(cratesCommon));
    printf("Number of uncommon crates: %d\n", json_object_get_int(cratesUncommon));
    printf("Number of rare crates: %d\n", json_object_get_int(cratesRare));
    printf("Number of magic crates: %d\n", json_object_get_int(cratesMagic));
    printf("Number of legendary crates: %d\n", json_object_get_int(cratesLegendary));
    printf("Current luck: %0.1f\n", json_object_get_double(luck));
    printf("God: %s\n", json_object_get_string(god));
    printf("Favor: %d\n", json_object_get_int(favor));
    printf("Race: %s\n", json_object_get_string(race));
    printf("CV: %d\n", json_object_get_int(cv));

    // Free some memory
    if( output.buffer )
    {
        free( output.buffer );
        output.buffer = 0;
        output.size = 0;
    }

    return 0;
}
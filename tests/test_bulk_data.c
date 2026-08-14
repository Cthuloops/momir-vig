#include <stdio.h>
#include <stdlib.h>

#include "../vendor/Unity/unity.h"
#include "../vendor/cJSON/cJSON.h"

#include "../src/bulk_data.c"

static const char *json_strings[] = {
  "{\"object\":\"bulk_data\",\"id\":\"e2ef41e3-5778-4bc2-af3f-"
  "78eca4dd9c23\",\"type\":\"default_cards\",\"updated_at\":\"2026-08-"
  "13T09:05:26.070+00:00\",\"uri\":\"https://api.scryfall.com/bulk-data/"
  "e2ef41e3-5778-4bc2-af3f-78eca4dd9c23\",\"name\":\"Default "
  "Cards\",\"description\":\"A JSON file containing every card object on "
  "Scryfall in English or the printed language if the card is only "
  "available in one "
  "language.\",\"jsonl_download_uri\":\"https://data.scryfall.io/"
  "default-cards/"
  "default-cards-20260813090526.jsonl.gz\",\"compressed_size\":77512052}",
};

enum JsonType
{
  GOOD_JSON
};

#define json_type_size 1

static cJSON *json_arr[json_type_size] = { 0 };

void
init_json (void)
{
  for (int i = 0; i < json_type_size; i++)
    {
      json_arr[i] = cJSON_Parse (json_strings[i]);
      if (!json_arr[i])
        {
          const char *err = cJSON_GetErrorPtr ();
          if (!err)
            {
              fprintf (stderr, "Error in json string %d before %s", i,
                       json_strings[i]);
              exit (EXIT_FAILURE);
            }
        }
    }
}

void
deinit_json (void)
{
  for (int i = 0; i < json_type_size; i++)
    {
      cJSON_Delete (json_arr[i]);
    }
}

static struct BulkDataItem *bdi = NULL;

void
setUp (void)
{
  bdi = malloc (sizeof (struct BulkDataItem));
  if (bdi)
    {
      memset (bdi, 0, sizeof (struct BulkDataItem));
    }
}

void
tearDown (void)
{
  if (bdi)
    {
      bulk_item_free (bdi);
      bdi = NULL;
    }
}

void
test_fill_bulk_item_type_should_match (void)
{
  fill_bulk_item_type (bdi, json_arr[GOOD_JSON]);

  TEST_ASSERT_NOT_NULL_MESSAGE (bdi, "BDI is NULL");
  TEST_ASSERT_NOT_NULL_MESSAGE (bdi->type_name, "type_name is NULL");
  TEST_ASSERT_EQUAL_STRING (bdi->type_name, "default_cards");
  TEST_ASSERT_EQUAL_INT_MESSAGE (bdi->type, SCRYFALL_BULK_DEFAULT_CARDS,
                                 "type enum doesn't match");
}

int
main (void)
{
  log_init (NULL);
  log_set_level (MOMIR_LOG_DEBUG);

  UNITY_BEGIN ();
  init_json ();
  RUN_TEST (test_fill_bulk_item_type_should_match);
  deinit_json ();

  log_deinit ();
  return UNITY_END ();
}

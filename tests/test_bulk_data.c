#include "../vendor/Unity/unity.h"

#include "../src/bulk_data.h"

enum TestCase
{
  TEST_CASE_VALID_JSON,
  TEST_CASE_UNKNOWN_TYPE
};

struct TestFixture
{
  const char json_input[1024];
  struct
  {
    int type;
    const char type_name[32];
    int64_t updated_at;
    uint64_t jsonl_size;
    const char uri[128];
    const char jsonl_download_uri[128];
  } expected;
};

static const struct TestFixture test_cases[] = {
  { .json_input
    = "{\"object\":\"bulk_data\",\"id\":\"e2ef41e3-5778-4bc2-af3f-"
      "78eca4dd9c23\",\"type\":\"default_cards\",\"updated_at\":\"2026-08-"
      "13T09:05:26.070+00:00\",\"uri\":\"https://api.scryfall.com/bulk-data/"
      "e2ef41e3-5778-4bc2-af3f-78eca4dd9c23\",\"name\":\"Default "
      "Cards\",\"description\":\"A JSON file containing every card object on "
      "Scryfall in English or the printed language if the card is only "
      "available in one "
      "language.\",\"jsonl_download_uri\":\"https://data.scryfall.io/"
      "default-cards/"
      "default-cards-20260813090526.jsonl.gz\",\"compressed_size\":"
      "77512052}",
    .expected
    = { .type = SCRYFALL_BULK_DEFAULT_CARDS,
        .type_name = "default_cards",
        .updated_at = 1786629926L,
        .jsonl_size = 77512052,
        .uri = "https://api.scryfall.com/bulk-data/"
               "e2ef41e3-5778-4bc2-af3f-78eca4dd9c23",
        .jsonl_download_uri = "https://data.scryfall.io/default-cards/"
                              "default-cards-20260813090526.jsonl.gz" } },
  { .json_input
    = "{\"type\":\"unknown_type\",\"updated_at\":\"2026-08-13T12:12:12\","
      "\"uri\":\"https://example.com\",\"jsonl_download_uri\":\"https://"
      "example.com/jsonl\",\"compressed_size\":121212}",
    .expected = { .type = SCRYFALL_BULK_UNKNOWN_CARDS,
                  .type_name = "unknown_type",
                  .updated_at = 1786641132L,
                  .jsonl_size = 121212,
                  .uri = "https://example.com",
                  .jsonl_download_uri = "https://example.com/jsonl" } },
};

void
setUp (void)
{
}

void
tearDown (void)
{
}

void
test_bulk_item_from_string_valid_json (void)
{
  const struct TestFixture *tc = &test_cases[TEST_CASE_VALID_JSON];
  struct BulkDataItem *item = bulk_item_from_string (tc->json_input);

  TEST_ASSERT_NOT_NULL (item);
  TEST_ASSERT_EQUAL_INT (tc->expected.type, item->type);
  TEST_ASSERT_EQUAL_STRING (tc->expected.type_name, item->type_name);
  TEST_ASSERT_EQUAL_INT64 (tc->expected.updated_at, (int64_t)item->updated_at);
  TEST_ASSERT_EQUAL_UINT64 (tc->expected.jsonl_size,
                            (uint64_t)item->jsonl_size);
  TEST_ASSERT_EQUAL_STRING (tc->expected.uri, item->uri);
  TEST_ASSERT_EQUAL_STRING (tc->expected.jsonl_download_uri,
                            item->jsonl_download_uri);

  bulk_item_free (item);
}

void
test_bulk_item_from_string_unknown (void)
{
  const struct TestFixture *tc = &test_cases[TEST_CASE_UNKNOWN_TYPE];
  struct BulkDataItem *item = bulk_item_from_string (tc->json_input);

  TEST_ASSERT_NOT_NULL (item);
  TEST_ASSERT_EQUAL_INT (tc->expected.type, item->type);
  TEST_ASSERT_EQUAL_STRING (tc->expected.type_name, item->type_name);
  TEST_ASSERT_EQUAL_INT64 (tc->expected.updated_at, (int64_t)item->updated_at);
  TEST_ASSERT_EQUAL_UINT64 (tc->expected.jsonl_size,
                            (uint64_t)item->jsonl_size);
  TEST_ASSERT_EQUAL_STRING (tc->expected.uri, item->uri);
  TEST_ASSERT_EQUAL_STRING (tc->expected.jsonl_download_uri,
                            item->jsonl_download_uri);
}

int
main (void)
{
  UNITY_BEGIN ();

  return UNITY_END ();
}

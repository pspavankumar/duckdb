#include "duckdb/main/capi_internal.hpp"

void duckdb_destroy_value(duckdb_value *value) {
	if (value && *value) {
		auto val = (duckdb::Value *)*value;
		delete val;
		*value = nullptr;
	}
}

duckdb_value duckdb_create_varchar(const char *text) {
	return (duckdb_value) new duckdb::Value(text);
}

duckdb_value duckdb_create_int64(int64_t input) {
	auto val = duckdb::Value::BIGINT(input);
	return (duckdb_value) new duckdb::Value(val);
}

char *duckdb_get_varchar(duckdb_value value) {
	auto val = (duckdb::Value *)value;
	auto str_val = val->CastAs(duckdb::LogicalType::VARCHAR);
	auto &str = duckdb::StringValue::Get(str_val);

	auto result = (char *)malloc(sizeof(char *) * (str.size() + 1));
	memcpy(result, str.c_str(), str.size());
	result[str.size()] = '\0';
	return result;
}

int64_t duckdb_get_int64(duckdb_value value) {
	auto val = (duckdb::Value *)value;
	if (!val->TryCastAs(duckdb::LogicalType::BIGINT)) {
		return 0;
	}
	return duckdb::BigIntValue::Get(*val);
}

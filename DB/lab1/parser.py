import pandas as pd

# 1) Загрузка CSV
path = "games.csv"
df = pd.read_csv(path)

# 2) Словарь маппинга pandas.dtype → Postgres
dtype_map = {
    'int64':    'BIGINT',
    'float64':  'DOUBLE PRECISION',
    'bool':     'BOOLEAN',
    'datetime64[ns]': 'TIMESTAMP',
    'object':   'TEXT'
}

# 3) Генерация частей для каждого столбца
cols = []
for col, dt in df.dtypes.astype(str).items():
    pg_type = dtype_map.get(dt, 'TEXT')  # по умолчанию TEXT
    cols.append(f'    "{col}" {pg_type}')

# 4) Собираем итоговый CREATE TABLE
table_name = 'bets'
create_sql = f'CREATE TABLE IF NOT EXISTS {table_name} (\n' + ",\n".join(cols) + "\n);\n"

print(create_sql)

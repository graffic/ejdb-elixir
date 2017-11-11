defmodule EjdbTest do
  use ExUnit.Case

  @database_filename "tmp/testdb"
  @collection_filename "tmp/testdb_potato"
  setup :clean_db
  def clean_db(_) do
    File.rm(@database_filename)
    File.rm(@collection_filename)
    :ok
  end

  test "version" do
    assert Ejdb.version() === "1.2.12"
  end

  test "create collection returns {:ok, collection}" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    {:ok, coll} = Ejdb.create_collection(db, "potato")
    assert is_reference coll
  end

  test "create a collection file" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    {:ok, _} = Ejdb.create_collection(db, "potato")
    assert(File.exists?(@collection_filename), "Collection file not created")
  end

  test "create collection with compressed" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    {:ok, coll} = Ejdb.create_collection(
      db, "potato", compressed: true, large: true, records: 1024, cachedrecords: 1024)
    assert is_reference coll
  end
end

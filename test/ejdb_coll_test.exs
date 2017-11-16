defmodule EjdbTest.Coll do
  use ExUnit.Case

  @database_filename "tmp/testdb"
  @collection_filename "tmp/testdb_potato"
  setup :clean_db
  def clean_db(_) do
    File.rm(@database_filename)
    File.rm(@collection_filename)
    :ok
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

  test "create collection with parameters" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    {:ok, coll} = Ejdb.create_collection(
      db, "potato", compressed: true, large: true, records: 1024, cachedrecords: 1024)
    assert is_reference coll
  end

  test "save to a collection" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    {:ok, coll} = Ejdb.create_collection(db, "potato")

    # At this point if we force GC, db would be freed as there is no other use
    # in the test. Unless colls adds a reference to db.
    # This GC tests that coll did a keep_resource on db.
    :erlang.garbage_collect

    {:ok, oid} = Ejdb.save_bson(coll, Bson.encode %{spam: "eggs"})
    assert String.length(oid) == 24
  end
end

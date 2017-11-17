defmodule EjdbTest.Coll do
  use ExUnit.Case

  @database_filename "tmp/testdb"
  @collection_filename "tmp/testdb_potato"
  setup :clean_db
  def clean_db(_) do
    File.rm(@database_filename)
    File.rm(@collection_filename)

    {:ok, db} = Ejdb.open @database_filename, [:jbocreat, :jbowriter]
    on_exit fn ->
      Ejdb.close(db)
    end

    {:ok, db: db}
  end

  test "create collection returns {:ok, collection}", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"
    assert is_reference coll
  end

  test "create a collection file", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"
    assert(File.exists?(@collection_filename), "Collection file not created")
  end

  test "create collection with parameters", context do
    {:ok, coll} = Ejdb.create_collection(
      context[:db], "potato", compressed: true, large: true, records: 1024, cachedrecords: 1024)
    assert is_reference coll
  end

  test "create collection wrong db parameter" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.create_collection "spam", 42
    end
  end

  test "create collection wrong coll name parameter", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.create_collection context.db, 42
    end
  end

  test "create collection with wrong options", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.create_collection context.db, "potato", 42
    end
  end

  test "create collection with invalid options", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato", [42, {42}, {42, 42}]
  end

  test "get a non existent collection", context do
    {:error, msg} = Ejdb.get_collection context.db, "potato"
    assert msg == "Collection does not exist"
  end

  test "get an existing collection", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"
    {:ok, coll} = Ejdb.get_collection context.db, "potato"
    assert is_reference coll
  end

  test "save to a collection", context do
    {:ok, coll} = Ejdb.create_collection(context.db, "potato")

    # At this point if we force GC, db would be freed as there is no other use
    # in the test. Unless colls adds a reference to db.
    # This GC tests that coll did a keep_resource on db.
    :erlang.garbage_collect

    {:ok, oid} = Ejdb.save_bson(coll, Bson.encode %{spam: "eggs"})
    assert String.length(oid) == 24
  end

  test "save to a collection with wrong coll", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"

    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.save_bson "spam", Bson.encode %{spam: "eggs"}
    end
  end

  test "save to a collection with wrong bson", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"

    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.save_bson coll, 42
    end
  end
end

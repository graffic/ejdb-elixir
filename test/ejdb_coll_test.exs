defmodule EjdbTest.Coll.Template do
  use ExUnit.CaseTemplate

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

    {:ok, db: db, bson: Bson.encode %{spam: "eggs"}}
  end
end

defmodule EjdbTest.Coll.Create do
  use EjdbTest.Coll.Template
  @collection_filename "tmp/testdb_potato"

  test "create collection returns {:ok, collection}", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"
    assert is_reference coll
  end

  test "create collection file", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"
    assert File.exists?(@collection_filename), "Collection file not created"
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

  test "create collection wrong coll name", context do
    {:error, msg} = Ejdb.create_collection context.db, "a.dot"
    assert msg == "invalid collection name"
  end

  test "create collection with wrong options", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.create_collection context.db, "potato", 42
    end
  end

  test "create collection with invalid options", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato", [42, {42}, {42, 42}]
  end
end

defmodule EjdbTest.Coll.Get do
  use EjdbTest.Coll.Template

  test "get collection with wrong database" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.get_collection "spam", "potato"
    end
  end

  test "get collection with wrong collection", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.get_collection context.db, 42
    end
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
end

defmodule EjdbTest.Coll.GetAll do
  use EjdbTest.Coll.Template

  test "get collections with wrong database" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.get_collections "spam"
    end
  end
  
  test "get all collections (no collections)", context do
    [] = Ejdb.get_collections context.db
  end

  test "get all collections", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"

    [_] = Ejdb.get_collections context.db
  end
end

defmodule EjdbTest.Coll.Remove do
  use EjdbTest.Coll.Template

  test "Remove collection wrong database parameter" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.remove_collection "spam", "potato", true
    end
  end

  test "Remove collection wrong collection parameter", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.remove_collection context.db, 42, true
    end
  end

  test "Remove collection wrong unlink parameter", context do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.remove_collection context.db, "potato", "spam"
    end
  end

  test "Remove collection", context do
    {:ok, _} = Ejdb.create_collection context.db, "potato"
    :ok = Ejdb.remove_collection context.db, "potato", true
    [] = Ejdb.get_collections context.db
  end
end

defmodule EjdbTest.Coll.Save do
  use EjdbTest.Coll.Template

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

defmodule EjdbTest.Coll.Load do
  use EjdbTest.Coll.Template

  test "Load a saved bson from with specified `oid`", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"
    {:ok, oid} = Ejdb.save_bson coll, context.bson
    {:ok, bson} = Ejdb.load_bson coll, oid
    assert "eggs" == Bson.decode(bson).spam
  end

  test "Load from non existing `oid`", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"
    :not_found = Ejdb.load_bson coll, "123456789012345678901234"
  end

  test "Load with wrong oid", context do
    {:ok, coll} = Ejdb.create_collection context.db, "potato"

    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.load_bson coll, "12"
    end
  end
end

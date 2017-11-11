defmodule EjdbTest.DB do
  use ExUnit.Case
  
  @database_filename "tmp/testdb"
  setup :clean_db
  def clean_db(_) do
    File.rm @database_filename
    :ok
  end

  test "create db wrong filename" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.open 42, [:jbocreat, :jbowriter]
    end
  end

  test "create db wrong options" do
    assert_raise ArgumentError, "argument error", fn ->
      Ejdb.open @database_filename, 42
    end
  end

  test "create db no options" do
    {:error, _} = Ejdb.open @database_filename, []
  end

  test "create db file" do
    Ejdb.open @database_filename, [:jbocreat, :jbowriter]
    assert File.exists?(@database_filename), "Database not created"

    # Make sure free_db_resource runs
    :erlang.garbage_collect
  end

  test "open db returns {:ok, db}" do
    {:ok, db} = Ejdb.open(@database_filename, [:jbocreat, :jbowriter])
    assert is_reference db
  end

  test "fails to open a db {:error, message}" do
    {:error, msg} = Ejdb.open(@database_filename, [:jbocreat])
    assert msg == "file not found"
  end
end
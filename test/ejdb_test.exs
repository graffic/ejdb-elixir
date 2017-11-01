defmodule EjdbTest do
  use ExUnit.Case
  use Bitwise

  @database_filename "tmp/testdb"
  setup :clean_db
  def clean_db(_) do
    File.rm(@database_filename)
    :ok
  end

  test "version" do
    assert Ejdb.version() === "1.2.12"
  end

  test "open db" do
    Ejdb.open(@database_filename, bor(Ejdb.jbocreat(), Ejdb.jbowriter()))
    assert(File.exists?(@database_filename), "Database not created")

    # Make sure free_db_resource runs
    :erlang.garbage_collect
  end

  test "open db returns {:ok, db}" do
    {:ok, db} = Ejdb.open(@database_filename, bor(Ejdb.jbocreat(), Ejdb.jbowriter()))
    assert is_reference db
  end
end

package role;

public abstract class Role {
    private String shared_buffer_name = "//localhost:1099/Buffer";

    abstract public void run();

    public String getSharedBufferName() {
        return shared_buffer_name;
    }
}

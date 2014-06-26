package burstio;

public interface ConnectionListener {
    public void portConnected (String connectionId);
    public void portDisconnected (String connectionId);
}

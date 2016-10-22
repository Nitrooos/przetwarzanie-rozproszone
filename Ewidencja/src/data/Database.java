package data;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Map;

public interface Database extends Remote {
    Person create(Person p) throws RemoteException;                // C

    Map<Integer, Person> getAll() throws RemoteException;          // R
    Person get(int id) throws RemoteException;
    Map<Integer, Person> search(String query) throws RemoteException;
    Integer findId(Person person) throws RemoteException;

    Person update(int id, Person update) throws RemoteException;   // U

    Person delete(int id) throws RemoteException;                  // D
}

package data;

import java.io.Serializable;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class BaseInfo implements Serializable {
    public enum Gender {
        MALE, FEMALE;

        static public Gender parse(String input) {
            switch (input) {
                case "M": return MALE;
                case "K": return FEMALE;
                default: throw new RuntimeException("Nieprawidłowy symbol: " + input + "\nUżyj M lub K\n");
            }
        }
    }

    private String _firstname;
    private String _lastname;
    private Date _birth;
    private Gender _gender;

    public BaseInfo(String first, String last, Date birth, Gender gender) {
        this._firstname = first;
        this._lastname = last;
        this._birth = birth;
        this._gender = gender;
    }

    public boolean match(String query) {
        Pattern p = Pattern.compile(query);
        Matcher m1 = p.matcher(this._firstname);
        Matcher m2 = p.matcher(this._lastname);
        return m1.find() || m2.find();
    }

    public String getFirstname() {
        return _firstname;
    }

    public String getLastname() {
        return _lastname;
    }

    public Date getBirth() {
        return _birth;
    }

    public Gender getGender() {
        return this._gender;
    }
}

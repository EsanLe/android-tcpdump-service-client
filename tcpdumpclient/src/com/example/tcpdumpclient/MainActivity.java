package com.example.tcpdumpclient;

import android.support.v7.app.ActionBarActivity;
import android.graphics.Point;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Toast;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.text.Editable;
import android.text.TextWatcher;

public class MainActivity extends ActionBarActivity {
    private Thread QueryThread;
    
    // The views and widgets
    private TextView textElement;
    private Spinner spinnerApi;
    private EditText editTextArg0;
    private EditText editTextArg1;
    
    // Default values
    private final String defaultIp = "10.0.2.15";
    private final String defaultIpTo = "10.0.2.2";
    private final String defaultPort = "5555";
    private final String defaultMac = "52:54:00:12:34:56";
    private final String defaultMacTo = "52:54:00:12:35:02";
    private final String defautProtool = "icmp";
    
    private String arg0 = "10.0.2.15";
    private String arg1 = defaultIpTo;
    private String code = "1";
    
    // The uiHander to control UI
    private Handler uiHandler = new UIHandler();

    class UIHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            // a message is received; update UI text view
            textElement.setText(msg.obj.toString());
            super.handleMessage(msg);
        }
    }
	
	static {
		System.loadLibrary("tcpdumpclient"); // "myjni.dll" in Windows, "libmyjni.so" in Unixes
	}

	public native String query(String code, String arg0, String arg1);
	
	@Override
	protected void onCreate(Bundle savedInstanceState)  {
		
		// Routine of onCreate
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		// Find views and widgets
	    textElement = (MonoTextView) findViewById(R.id.mytext);
	    editTextArg0 = (EditText) findViewById(R.id.arg0);
	    editTextArg1 = (EditText) findViewById(R.id.arg1);
	    spinnerApi = (Spinner) findViewById(R.id.api);
	    
	    // Set the default values
	    editTextArg0.setText(defaultIp);
	    editTextArg1.setText("");
	}

	@Override
	protected void onResume() {
		super.onResume();
		
		// Register Listeners
		addListenerToArg0();
		addListenerToArg1();
		addListenerToApi();
		
		// Start the query thread
		QueryThread = getNewQueryThread();
		QueryThread.start();
	}



	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	// Verify method
	private boolean isIp(String ip){
		int PointCount = 0;
		int Number = 0;
		for(int i = 0; i < ip.length(); i++){
			char CurrentChar = ip.charAt(i);
			if (CurrentChar < '9' && CurrentChar > '0'){
				Number = Number * 10 + CurrentChar - '0';
				if (Number >= 255){
					return false;
				}
			}
			if (CurrentChar == '.'){
				PointCount++;
				Number = 0;
			}
		}
		if (Number == 0) return false;
		if (PointCount != 3 ) return false;
		return true;
	}
	
	
	// Thread methods
	private Thread getNewQueryThread(){
		 return new Thread() {
			public void run() {
				for (;;){
					// create message which will be send to handler
					Message msg = Message.obtain(uiHandler);
		//			msg.obj = query("1", "10.0.2.15", "arg1");
					msg.obj = query(code, arg0, arg1);
					uiHandler.sendMessage(msg);
				}
			}
		};
	}
	
	private void restartThread(Thread thread){
//		thread.interrupt();
//		thread = null;
//		thread = getNewQueryThread();
//		thread.start();
	}
	
	// Listener methods
	private void addListenerToArg0(){
		editTextArg0.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
	            String text = editTextArg0.getText().toString();
	            if (isIp(text)) {
	            	arg0 = text;
	            	restartThread(QueryThread);
	            }
			}
		});
	}
	
	private void addListenerToArg1(){
		editTextArg0.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
	            String text = editTextArg1.getText().toString();
	            if (isIp(text)) {
	            	arg1 = text;
	            	restartThread(QueryThread);
	            }
			}
		});
	}
	
	private void addListenerToApi() {

		spinnerApi.setOnItemSelectedListener(new OnItemSelectedListener() {
		    Boolean isLoaded = false;
		    @Override
		    public void onItemSelected(AdapterView<?> arg, View argx, int argxx, long argxxx) {
		        if(isLoaded){
		        	int api = spinnerApi.getSelectedItemPosition() + 1;
		        	code = Integer.toString(api);
		            Toast.makeText(getBaseContext(),
		                    "Api" + code + ": " + spinnerApi.getSelectedItem(),
		                    Toast.LENGTH_SHORT).show();
//		            restartThread(QueryThread);
		            switch(api){
		            	// getDataPacketFromIP
		            	case 1:
		            	    editTextArg0.setText(defaultIp);
		            	    editTextArg1.setText("");
		            	    arg0 = defaultIp;
		            		break;
		            		
		            	//	getDataPacketToIP
		            	case 2:
		            	    editTextArg0.setText("");
		            	    editTextArg1.setText(defaultIp);
		            	    arg0 = defaultIp;
		            		break;
		            	
		            	// getDataPacketFromIPAndPort
		            	case 3:
		            	    editTextArg0.setText(defaultIp);
		            	    editTextArg1.setText(defaultPort);
		            	    arg0 = defaultIp;
		            	    arg1 = defaultPort;
		            		break;
		            	
		            	// getDataPacketFromPort
		            	case 4:
		            	    editTextArg0.setText(defaultPort);
		            	    editTextArg1.setText("");
		            	    arg0 = defaultPort;
		            		break;
		            	
		            	// getDataPacketToPort
		            	case 5:
		            	    editTextArg0.setText("");
		            	    editTextArg1.setText(defaultPort);
		            	    arg0 = defaultPort;
		            		break;
		            	
		            	// getDataPacketFromIPToIP
		            	case 6:
		            	    editTextArg0.setText(defaultIp);
		            	    editTextArg1.setText(defaultIpTo);
		            	    arg0 = defaultIp;
		            	    arg1 = defaultIpTo;
		            		break;
		            	
		            	// getDataPacketFromMac
		            	case 7:
		            	    editTextArg0.setText(defaultMac);
		            	    editTextArg1.setText("");
		            	    arg0 = defaultMac;
		            		break;
		            		
		            	// getDataPacketToMac
		            	case 8:
		            	    editTextArg0.setText("");
		            	    editTextArg1.setText(defaultMac);
		            	    arg0 = defaultMac;
		            		break;
		            	
		            	// getDataPacketFromMacToMac
		            	case 9:
		            	    editTextArg0.setText(defaultMac);
		            	    editTextArg1.setText(defaultMacTo);
		            	    arg0 = defaultMac;
		            	    arg1 = defaultMacTo;
		            		break;
		            	

		            	// getDataPacketWithProtocol
		            	case 10:
		            	    editTextArg0.setText(defautProtool);
		            	    editTextArg1.setText("");
		            	    arg0 = defautProtool;
		            		break;
		            }
		        }
		        isLoaded = true;
		    }
			@Override
			public void onNothingSelected(AdapterView<?> parent) {
				// TODO Auto-generated method stub
			}
		});
	}
}

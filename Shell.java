import java.util.Arrays;
import java.util.Set;
import java.util.TreeSet;

class Shell extends Thread {

	public Shell() {
	}

	public Shell(String args[]) {	
	}

	public void run() {
		// Process counter starts at 1, will increment each time a command is run. 
		int processCount = 1; 
		// Could just set to true, but like having a naming convention. 
		boolean running = true; 

		while(running) {
			// Shell Prompt. 
			SysLib.cout("shell[" + processCount + "]% " ); 
			String[] singleCommand; 
			// Needed for CIN to work. 
			StringBuffer sb = new StringBuffer();
            SysLib.cin(sb);
            
            // String of what is sent from the CIN. 
            String commandStr = new String(sb);

            // If command is exit, string.compareTo will return 0, so we can use
            // it to exit. 
            if (commandStr.compareTo("exit") == 0) {
            	break; 
            }
            
            if (!commandStr.isEmpty()) {
            	processCount++; 
            	Set<Integer> thread = new TreeSet<Integer>(); 
            	int threadNumber; 
            	String[] mainArray = commandStr.split(";");
			    String[][] result = new String[mainArray.length][];
			    int count = 0;

			    for (String t : mainArray) {
			        result[count] = t.split("&");
			        count++;
			    }
			    for (int semi = 0; semi < result.length; semi++) {
			    	for (int and = 0; and < result[semi].length; and++) {
			    		String command = result[semi][and];

		           		String[] execution = SysLib.stringToArgs(command);

		           		if (execution.length == 0) {
		           			continue; 
		           		}

		           		if ((threadNumber = SysLib.exec(execution)) > -1) {
		           			thread.add(threadNumber);
		           		}
				    }
		    		while(!thread.isEmpty()) {
			    		threadNumber = SysLib.join(); 
			    		if (thread.contains(threadNumber)) {
			    			thread.remove(threadNumber);
			    		}
			    	}
			    }
            }
			SysLib.exit();
		}
	} 
}

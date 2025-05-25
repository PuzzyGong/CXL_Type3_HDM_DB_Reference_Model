class test extends uvm_test;
    `uvm_component_utils(test)

    function new(string name = "test", uvm_component parent = null);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        $display("\n\n--------------- UVM_TEST START SUCCESSFUL ---------------\n");
    endfunction

    task run_phase(uvm_phase phase);
        phase.raise_objection(this);
        #600;
        $display("\n--------------- UVM_TEST END SUCCESSFUL ---------------\n\n");
        phase.drop_objection(this);
    endtask
endclass

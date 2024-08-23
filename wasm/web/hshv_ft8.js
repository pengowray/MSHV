let hshvModule = {
    _init_ft8: null,
    _pack_ft8_message: null,
    _unpack_ft8_message: null,
    _save_hash_call: null,
    
    init: function() {
        return new Promise((resolve) => {
            Module.onRuntimeInitialized = () => {
                this._init_ft8 = Module.cwrap('init_ft8', null, []);
                this._pack_ft8_message = Module.cwrap('pack_ft8_message', 'string', ['string']);
                this._unpack_ft8_message = Module.cwrap('unpack_ft8_message', 'string', ['string']);
                this._save_hash_call = Module.cwrap('save_hash_call', null, ['string']);
                
                this._init_ft8();
                resolve();
            };
        });
    },
    
    packMessage: function(message) {
        return this._pack_ft8_message(message);
    },
    
    unpackMessage: function(packedMessage) {
        return this._unpack_ft8_message(packedMessage);
    },
    
    saveHashCall: function(call) {
        this._save_hash_call(call);
    }
};

// Usage example:
async function main() {
    await ft8Module.init();
    
    const message = "CQ K1ABC FN42";
    const packedMessage = ft8Module.packMessage(message);
    console.log("Packed message:", packedMessage);
    
    const unpackedMessage = ft8Module.unpackMessage(packedMessage);
    console.log("Unpacked message:", unpackedMessage);
    
    ft8Module.saveHashCall("K1ABC");
}

main();
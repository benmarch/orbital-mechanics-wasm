class App extends HTMLElement {
    static get template() {
        return html`
            <style>
                .wide-column {
                    width: 40%;
                    float: left;
                }
                .narrow-column {
                    width: 20%;
                    float: left;
                }
            </style>

            <div class="wide-column">
                <state-vector-generator id="stateVectorGenerator"></state-vector-generator>
            </div>
            <div class="narrow-column">
                <button id="transferStateVectorsButton" onclick="this.getRootNode().host.transferStateVectors()">Copy State Vectors --></button>
                <label>
                    <span>Auto</span>
                    <input type="checkbox" onchange="this.getRootNode().host.toggleAutoTransferSV(event)">
                </label>
                <br>
                <button id="transferOrbitalElementsButton" onclick="this.getRootNode().host.transferOrbitalElements()"><-- Copy Orbital Elements</button>
                <label>
                    <span>Auto</span>
                    <input type="checkbox" onchange="this.getRootNode().host.toggleAutoTransferOE(event)">
                </label>
            </div>
            <div class="wide-column">
                <orbital-elements-generator id="orbitalElementsGenerator"></orbital-elements-generator>
            </div>
        `;
    }

    shouldAutoTransferSV = false;
    shouldAutoTransferOE = false;

    constructor() {
        super();

        const shadowRoot = this.attachShadow({mode:'open'});
        shadowRoot.appendChild(document.importNode(App.template.content, true));

        this.stateVectorGenerator = shadowRoot.getElementById('stateVectorGenerator');
        this.elementsGenerator = shadowRoot.getElementById('orbitalElementsGenerator');
    }

    connectedCallback() {
        this.stateVectorGenerator.addEventListener('input', event => {
            if (this.shouldAutoTransferSV) {
                this.transferStateVectors();
            }
        });

        this.elementsGenerator.addEventListener('input', event => {
            if (this.shouldAutoTransferOE) {
                this.transferOrbitalElements();
            }
        });
    }

    transferStateVectors() {
        const stateVectors = this.stateVectorGenerator.getStateVectors();
        this.elementsGenerator.updateWithStateVectors(stateVectors);
    }

    transferOrbitalElements() {
        const elements = this.elementsGenerator.getElements();
        this.stateVectorGenerator.updateWithOrbitalElements(elements);
    }

    toggleAutoTransferSV(event) {
        this.shouldAutoTransferSV = event.target.checked;
    }

    toggleAutoTransferOE(event) {
        this.shouldAutoTransferOE = event.target.checked;
    }
}

// upgrade after WASM loads
Module.onRuntimeInitialized = () => {
    window.customElements.define('om-app', App);
}
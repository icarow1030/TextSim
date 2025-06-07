import React from 'react';
import { FaNetworkWired, FaCog, FaPlay, FaComments, FaUserEdit, FaInfoCircle } from 'react-icons/fa';
import './ManualView.css';

// Componente auxiliar para padronizar cada passo da instrução
const InstructionStep: React.FC<{ number: number; title: string; icon: React.ReactNode; children: React.ReactNode }> = ({ number, title, icon, children }) => {
  return (
    <div className="instruction-step">
      <div className="step-header">
        <div className="step-number">{number}</div>
        <div className="step-icon">{icon}</div>
        <h3>{title}</h3>
      </div>
      <div className="step-content">
        {children}
      </div>
    </div>
  );
};


const ManualView: React.FC = () => {
    return (
        <div className="manual-view">
            <h2>Manual de Utilização do TextSim</h2>
            <p className="manual-intro">
                Siga estes passos para estabelecer uma comunicação segura entre duas instâncias da aplicação.
            </p>

            <InstructionStep number={1} title="Abra Duas Instâncias" icon={<FaNetworkWired />}>
                <p>Para simular uma conversa, você precisa de dois "participantes". Abra duas janelas do <strong>TextSim</strong> no seu computador. Vamos chamá-las de <strong>App 1</strong> e <strong>App 2</strong>.</p>
            </InstructionStep>

            <InstructionStep number={2} title="Configure as Portas" icon={<FaCog />}>
                <p>A comunicação precisa de um endereço. Cada app precisa saber a "porta" do outro. Vá para <strong>Configurações de Conexão</strong> (<FaCog />) em ambos os apps.</p>
                <div className="port-config-diagram">
                    <div className="app-box">
                        <h4>App 1</h4>
                        <p>Sua Porta: <code>8071</code></p>
                        <p>Porta de Destino: <code>8070</code></p>
                    </div>
                    <div className="arrow">→</div>
                    <div className="app-box">
                        <h4>App 2</h4>
                        <p>Sua Porta: <code>8070</code></p>
                        <p>Porta de Destino: <code>8071</code></p>
                    </div>
                </div>
                <div className="info-box">
                    <FaInfoCircle />
                    <span>A <strong>Porta de Destino</strong> de um app deve ser a <strong>Sua Porta</strong> do outro.</span>
                    <span>Pelo menos um deles, pois haverá o<strong> handshake </strong>que irá configurar o outro de acordo.</span>
                </div>
            </InstructionStep>
            
            <InstructionStep number={3} title="Inicie os Servidores" icon={<FaPlay />}>
                <p>Ambos os aplicativos precisam estar "escutando" por conexões. Em <strong>ambos os apps</strong>, vá para a tela <strong>Servidor</strong> (<FaPlay style={{color: 'var(--red)'}}/>) e clique em <strong>"Iniciar Servidor"</strong>. O status de ambos deve mudar para "Online".</p>
            </InstructionStep>

            <InstructionStep number={4} title="Inicie o Chat e o Handshake" icon={<FaComments />}>
                <p>Com os servidores online, é hora de conectar. No <strong>App 1</strong>, vá para a tela de <strong>Chat</strong> (<FaComments />). O simples ato de entrar nesta tela (ou enviar a primeira mensagem) iniciará o <strong>handshake</strong>.</p>
                <p>O handshake é o processo automático onde os apps trocam suas chaves públicas RSA. Você pode observar este processo em detalhes na tela de <strong>Logs</strong>.</p>
            </InstructionStep>

            <InstructionStep number={5} title="Configurações Opcionais" icon={<FaUserEdit />}>
                <p>Personalize sua experiência. Em <strong>Configurações de Usuário</strong> (<FaUserEdit />), você pode:</p>
                <ul>
                    <li><strong>Editar seu nome de usuário:</strong> Este nome aparecerá para o seu contato.</li>
                    <li><strong>Gerar novas chaves RSA:</strong> Para uma camada extra de segurança, você pode gerar um novo par de chaves a qualquer momento antes de iniciar um chat.</li>
                </ul>
            </InstructionStep>
        </div>
    );
};

export default ManualView;